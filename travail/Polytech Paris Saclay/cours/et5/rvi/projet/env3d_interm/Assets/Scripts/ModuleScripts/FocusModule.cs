using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FocusModule : MonoBehaviour
{
    public Transform targetObject;  // L'objet entier à déplacer
    public Camera vrCamera;         // La caméra VR (si non assignée, prend la caméra principale)
    public float distanceFromCamera = 2.0f; // Distance entre l'objet et la caméra

    void Start()
    {
        if (vrCamera == null)
        {
            vrCamera = Camera.main;
        }

        if (vrCamera == null)
        {
            Debug.LogError("Aucune caméra trouvée. Assignez une caméra manuellement.");
            return;
        }

        if (targetObject == null)
        {
            Debug.LogError("Aucun targetObject assigné. Assignez l'objet à déplacer dans l'inspecteur.");
            return;
        }
    }

    void Update()
    {
        // Détecte un clic gauche ou un appui sur le bouton principal du contrôleur
        if (Input.GetMouseButtonDown(0)) // Pour la souris ; pour le VR, utilisez un autre système d'entrée
        {
            PerformRaycast();
        }
    }

    private void PerformRaycast()
    {
        // Émet un rayon depuis la caméra en direction du point cliqué
        Ray ray = vrCamera.ScreenPointToRay(Input.mousePosition);
        RaycastHit hit;

        // Vérifie si le rayon touche cet objet (module)
        if (Physics.Raycast(ray, out hit))
        {
            if (hit.transform == transform) // Vérifie que l'objet cliqué est bien le module
            {
                MoveObjectToCameraCenter();
            }
        }
    }

    private void MoveObjectToCameraCenter()
    {
        // Calcule la nouvelle position de l'objet en fonction de la caméra
        Vector3 cameraForward = vrCamera.transform.forward;
        Vector3 cameraPosition = vrCamera.transform.position;

        // Positionne l'objet à une distance définie devant la caméra
        Vector3 targetPosition = cameraPosition + cameraForward * distanceFromCamera;

        // Déplace l'objet entier
        targetObject.position = targetPosition;

        // Oriente l'objet entier pour qu'il fasse face à la caméra
        targetObject.LookAt(vrCamera.transform, Vector3.up);
    }
}
