using System.Collections;
using UnityEngine;

public class ObjectLiftController : MonoBehaviour
{
    public Transform targetPosition; // Position où placer l'objet soulevé
    public float moveSpeed = 5f;     // Vitesse du déplacement
    public BombRotation bombRotation; // Référence au script de rotation
    public float rotationSpeed = 5f;  // Vitesse de rotation vers la caméra

    private bool isLifted = false;    // État de l'objet (soulevé ou non)
    private Vector3 originalPosition; // Position initiale de l'objet
    private Quaternion originalRotation; // Rotation initiale de l'objet

    public Vector3 liftedCameraPosition; // Position de la caméra lorsque la bombe est levée
    public Quaternion liftedCameraRotation; // Rotation de la caméra lorsque la bombe est levée

    void Start()
    {
        originalPosition = transform.position;
        originalRotation = transform.rotation;

        if (bombRotation != null)
        {
            bombRotation.enabled = false; // Désactiver la rotation libre dès le départ
        }
    }

    void Update()
    {
        // Interaction pour soulever l'objet
        if (Input.GetMouseButtonDown(0) && IsMouseOverObject() && !isLifted)
        {
            LiftObject();
        }

        // Interaction pour reposer l'objet au sol
        if (Input.GetMouseButtonDown(1) && isLifted)
        {
            LowerObject(); // Reposer la bombe
        }
    }

    public void LiftObject()
    {
        isLifted = true;

        // Sauvegarder la position de la caméra lorsque la bombe est levée
        liftedCameraPosition = Camera.main.transform.position;
        liftedCameraRotation = Camera.main.transform.rotation;

        // Déplacement fluide vers la position cible
        StartCoroutine(SmoothLiftMovement(targetPosition.position, Quaternion.LookRotation(Camera.main.transform.forward)));

        // Activer la rotation contrôlée par la souris
        if (bombRotation != null)
        {
            bombRotation.enabled = true;
        }
    }

    public void LowerObject()
    {
        isLifted = false;

        // Déplacement fluide vers la position initiale
        StartCoroutine(SmoothLiftMovement(originalPosition, originalRotation));

        // Désactiver la rotation contrôlée par la souris
        if (bombRotation != null)
        {
            bombRotation.enabled = false;
        }
    }

    IEnumerator SmoothLiftMovement(Vector3 targetPos, Quaternion targetRot)
    {
        float elapsedTime = 0f;
        float duration = 1f / moveSpeed; // Ajuster la durée selon la vitesse

        Vector3 startPos = transform.position;
        Quaternion startRot = transform.rotation;

        while (elapsedTime < duration)
        {
            float t = Mathf.SmoothStep(0f, 1f, elapsedTime / duration);

            // Mouvement et rotation fluide
            transform.position = Vector3.Lerp(startPos, targetPos, t);
            transform.rotation = Quaternion.Lerp(startRot, targetRot, t);

            elapsedTime += Time.deltaTime;
            yield return null;
        }

        // Assure la position et la rotation exactes
        transform.position = targetPos;
        transform.rotation = targetRot;
    }

    // Vérifie si la souris est sur l'objet
    bool IsMouseOverObject()
    {
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit hit;

        if (Physics.Raycast(ray, out hit))
        {
            return hit.collider.gameObject == gameObject;
        }
        return false;
    }

    // Retourne vrai si l'objet est soulevé
    public bool IsLifted()
    {
        return isLifted;
    }
}
