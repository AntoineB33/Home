using System.Collections;
using UnityEngine;

public class ModuleFocusController : MonoBehaviour
{
    public Transform focusPoint;      // Point de focus pour les modules
    public float focusSpeed = 5f;     // Vitesse de focus sur le module
    public ObjectLiftController liftController; // Référence au script de levage de la bombe

    private Transform currentModule;  // Le module actuellement sélectionné
    private bool isCameraFocusedOnModule = false; // Suivi de l'état de la caméra
    private Vector3 initialCameraPosition;  // Position initiale de la caméra
    private Quaternion initialCameraRotation; // Rotation initiale de la caméra

    void Start()
    {
        // Sauvegarder la position et la rotation initiale de la caméra
        initialCameraPosition = Camera.main.transform.position;
        initialCameraRotation = Camera.main.transform.rotation;
    }

    void Update()
    {
        // Si la bombe est levée, permet la sélection des modules
        if (liftController != null && liftController.IsLifted())
        {
            // Si l'utilisateur clique sur un module
            if (Input.GetMouseButtonDown(0))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;

                if (Physics.Raycast(ray, out hit))
                {
                    // Vérifier si l'objet touché est un module
                    if (hit.collider.CompareTag("Module"))
                    {
                        SelectModule(hit.collider.transform);
                    }
                }
            }

            // Si l'utilisateur clique droit
            if (Input.GetMouseButtonDown(1))
            {
                if (isCameraFocusedOnModule)
                {
                    // Si la caméra est focalisée sur le module, on revient à la vue de la bombe levée
                    ReturnToLiftedBombView();
                }
                else
                {
                    // Si la caméra est sur la bombe, on repose la bombe
                    LowerObjectToInitialPosition();
                }
            }
        }
    }

    // Sélectionner un module et recentrer la caméra dessus
    void SelectModule(Transform module)
    {
        if (module == currentModule) return; // Si c'est déjà le module sélectionné, ne rien faire

        currentModule = module;

        // Recentrer la caméra sur le module
        StartCoroutine(SmoothFocusMovement(module));
        isCameraFocusedOnModule = true; // La caméra est maintenant focalisée sur le module
    }

    // Retour à la vue de la bombe levée
    void ReturnToLiftedBombView()
    {
        StartCoroutine(SmoothReturnToLiftedBombView());
        isCameraFocusedOnModule = false; // La caméra n'est plus focalisée sur le module
    }

    // Repose la bombe à sa position initiale
    void LowerObjectToInitialPosition()
    {
        liftController.LowerObject();
        isCameraFocusedOnModule = false; // La caméra n'est plus focalisée sur le module
    }

    // Déplacement fluide de la caméra vers le module sélectionné
    IEnumerator SmoothFocusMovement(Transform module)
    {
        Vector3 targetPos = module.position + new Vector3(0, 0, -3f); // Décalage pour mieux zoomer sur le module
        Quaternion targetRot = Quaternion.LookRotation(module.position - Camera.main.transform.position);

        float elapsedTime = 0f;
        float duration = 1f / focusSpeed;

        Vector3 startPos = Camera.main.transform.position;
        Quaternion startRot = Camera.main.transform.rotation;

        while (elapsedTime < duration)
        {
            float t = Mathf.SmoothStep(0f, 1f, elapsedTime / duration);

            Camera.main.transform.position = Vector3.Lerp(startPos, targetPos, t);
            Camera.main.transform.rotation = Quaternion.Lerp(startRot, targetRot, t);

            elapsedTime += Time.deltaTime;
            yield return null;
        }

        Camera.main.transform.position = targetPos;
        Camera.main.transform.rotation = targetRot;
    }

    // Retourne la caméra à la vue où la bombe est levée
    IEnumerator SmoothReturnToLiftedBombView()
    {
        float elapsedTime = 0f;
        float duration = 1f / focusSpeed;

        Vector3 startPos = Camera.main.transform.position;
        Quaternion startRot = Camera.main.transform.rotation;

        while (elapsedTime < duration)
        {
            float t = Mathf.SmoothStep(0f, 1f, elapsedTime / duration);

            Camera.main.transform.position = Vector3.Lerp(startPos, liftController.liftedCameraPosition, t);
            Camera.main.transform.rotation = Quaternion.Lerp(startRot, liftController.liftedCameraRotation, t);

            elapsedTime += Time.deltaTime;
            yield return null;
        }

        Camera.main.transform.position = liftController.liftedCameraPosition;
        Camera.main.transform.rotation = liftController.liftedCameraRotation;
    }
}
