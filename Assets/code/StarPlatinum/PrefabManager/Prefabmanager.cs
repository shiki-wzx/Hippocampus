using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using SQLite4Unity3d;
using UnityEngine;
using UnityEngine.AddressableAssets;
using UnityEngine.ResourceManagement.AsyncOperations;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using Object = UnityEngine.Object;

namespace StarPlatinum
{
	public enum RequestStatus
	{
		SUCCESS,
		FAIL
	}

	public struct RequestResult
	{
		public Object result;
		public string key;
		public RequestStatus status;

		public RequestResult (Object result, string key, RequestStatus status)
		{
			this.key = key;
			this.result = result;
			this.status = status;
		}
	}
	public class PrefabManager : Singleton<PrefabManager>
	{

		Dictionary<string, GameObject> m_allPrefab = new Dictionary<string, GameObject> ();
		Dictionary<string, Object> m_objects = new Dictionary<string, Object> ();

		Dictionary<string, Action<RequestResult>> m_loadingCallback = new Dictionary<string, Action<RequestResult>> ();
		public GameObject LoadPrefab (string prefabName)
		{
			//string name = "Prefabs/" + prefabName;
			if (m_allPrefab.ContainsKey (prefabName)) {
				return m_allPrefab [prefabName];
			} else {
				GameObject perfb = null;
				if (perfb == null) {
					Debug.Log (prefabName + "can`t find in Prefabs/" + prefabName);
					return null;
				}
				m_allPrefab.Add (prefabName, perfb);
				return perfb;
			}
		}

		public void InstantiateAsync<T> (string key, Action<RequestResult> callBack, Transform parent = null) where T : UnityEngine.Object
		{
			Addressables.LoadAsset<T> (key).Completed += operation => {
				var result = GetResult (key, operation);
				if (result.status == RequestStatus.FAIL) {
					callBack?.Invoke (result);
					return;
				}
				result.result = Object.Instantiate (operation.Result, parent);
				callBack?.Invoke (result);
			};
		}

		public void InstantiateComponentAsync<T> (string key, Action<RequestResult> callBack, Transform parent = null) where T : UnityEngine.Object
		{
			Addressables.LoadAsset<GameObject> (key).Completed += operation => {
				var result = GetResult (key, operation);
				if (result.status == RequestStatus.FAIL) {
					callBack?.Invoke (result);
					return;
				}
				var obj = Object.Instantiate (operation.Result, parent);
				result.result = obj.GetComponent<T> ();
				callBack?.Invoke (result);
			};
		}

		public void InstantiateConfigAsync (string key, Action<RequestResult> callBack, Transform parent = null)
		{
			Addressables.LoadAsset<ScriptableObject> (key).Completed += operation => {
				var result = GetResult (key, operation);
				if (result.status == RequestStatus.FAIL) {
					callBack?.Invoke (result);
					return;
				}
				result.result = Object.Instantiate (operation.Result, parent);
				callBack?.Invoke (result);
			};
		}

		public void SetImage (Image image, string key, Action failCallBack = null)
		{
			LoadAssetAsync<Sprite> (key, (result) => {
				if (result.status == RequestStatus.FAIL) {
					SetImage (image, "Image_Default");
					failCallBack?.Invoke ();
					Debug.LogWarning ($"图片加载错误 Key:{result.key}");
					return;
				}
				image.sprite = result.result as Sprite;
			});
		}

		private void LoadAssetAsync<T> (string key, Action<RequestResult> callBack, Transform parent = null) where T : UnityEngine.Object
		{
			if (m_objects.ContainsKey (key)) {
				var result = new RequestResult (m_objects [key], key, RequestStatus.SUCCESS);
				callBack?.Invoke (result);
			}
			if (m_loadingCallback.ContainsKey (key)) {
				m_loadingCallback [key] += callBack;
				return;
			}
			Addressables.LoadAsset<T> (key).Completed += operation => {
				var result = GetResult (key, operation);
				m_loadingCallback [key].Invoke (result);
				m_loadingCallback.Remove (key);
				m_objects [key] = result.result;
			};
			m_loadingCallback [key] = callBack;
		}

		public void LoadAssetsAsync<T> (List<string> keys, Action<RequestResult> callBack, Transform parent = null) where T : UnityEngine.Object
		{
			foreach (var key in keys) {
				if (m_objects.ContainsKey (key)) {
					var result = new RequestResult (m_objects [key], key, RequestStatus.SUCCESS);
					callBack?.Invoke (result);
				}
				if (m_loadingCallback.ContainsKey (key)) {
					m_loadingCallback [key] += callBack;
					continue;
				}
				Addressables.LoadAsset<T> (key).Completed += operation => {
					var result = GetResult (key, operation);
					m_loadingCallback [key].Invoke (result);
					m_loadingCallback.Remove (key);
					m_objects [key] = result.result;
				};
				m_loadingCallback [key] = callBack;
			}
		}

		public void LoadScene (SceneLookupEnum key, LoadSceneMode loadSceneMode)
		{
			Addressables.LoadScene (SceneLookup.GetString (key), loadSceneMode);
			SceneManager.Instance ().SetCurrentScene (key);
		}

		private RequestResult GetResult<T> (string key, AsyncOperationHandle<T> operation) where T : UnityEngine.Object
		{
			RequestResult result = new RequestResult ();
			result.key = key;
			if (operation.Result == null) {
				result.status = RequestStatus.FAIL;
				return result;
			}
			result.result = operation.Result;
			result.status = RequestStatus.SUCCESS;
			return result;
		}


		public void UploadPrefab (string name)
		{
			GameObject temp = m_allPrefab [name];
			m_allPrefab.Remove (name);
			Object.Destroy (temp);
		}

		public void UploadAllPrefab ()
		{

			Dictionary<string, GameObject>.Enumerator etor = m_allPrefab.GetEnumerator ();
			while (etor.MoveNext ()) {

				GameObject.Destroy (etor.Current.Value);

			}
			m_allPrefab.Clear ();
		}
	}
}