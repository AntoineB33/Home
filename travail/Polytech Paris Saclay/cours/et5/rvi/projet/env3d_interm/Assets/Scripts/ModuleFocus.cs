using UnityEngine;

public class ModuleFocus : MonoBehaviour
{
    private bool isHovered = false;
    private bool isProcessing = false;
    private bool isFocused = false; // Le module est-il focusé ?
    private bool isMainObjectActive = true; // L'objet principal est-il actif et libre ?

    [SerializeField]
    GameObject TargetObject; // Le module à gérer

    private Outline Outline;

    // Gestion des positions et de la caméra
    public float focusDistance = 2f; // Distance entre la caméra et le module
    public float transitionSpeed = 2f; // Vitesse de transition de la caméra
    private Camera mainCamera;
    private Vector3 initialCameraPosition;
    private Quaternion initialCameraRotation;

    void Start()
    {
        // Gestion du surlignement
        Outline = TargetObject.GetComponent<Outline>();
        if (Outline == null)
        {
            Outline = TargetObject.AddComponent<Outline>();
        }
        Outline.enabled = false;

        // Récupération de la caméra principale
        mainCamera = Camera.main;
        initialCameraPosition = mainCamera.transform.position;
        initialCameraRotation = mainCamera.transform.rotation;
    }

    void Update()
    {
        // Vérifie si la souris est sur l'objet
        if (IsMouseOver())
        {
            if (!isHovered)
            {
                OnMouseEnter();
            }
        }
        else
        {
            if (isHovered)
            {
                OnMouseExit();
            }
        }

        // Gestion des clics
        if (isHovered && Input.GetMouseButtonDown(0)) // Clic gauche
        {
            if (!isFocused)
            {
                FocusOnModule();
            }
        }
        else if (Input.GetMouseButtonDown(1)) // Clic droit
        {
            HandleRightClick();
        }
    }

    bool IsMouseOver()
    {
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit))
        {
            return hit.collider.gameObject == gameObject;
        }
        return false;
    }

    void OnMouseEnter()
    {
        isHovered = true;
        if (!isProcessing)
        {
            HighlightBorders();
        }
    }

    void OnMouseExit()
    {
        isHovered = false;
        UnhighlightBorders();
    }

    void HighlightBorders()
    {
        isProcessing = true;
        Outline.enabled = true;
        Outline.OutlineColor = Color.red;
        Outline.OutlineWidth = 4;
        Outline.OutlineMode = Outline.Mode.OutlineAll;
    }

    void UnhighlightBorders()
    {
        isProcessing = false;
        Outline.enabled = false;
    }

    void FocusOnModule()
    {
        if (!isMainObjectActive) return;

        isFocused = true;
        isMainObjectActive = false;

        BombRotation.IsModuleFocused = true; // Désactive la rotation de la bombe

        Debug.Log("Focus sur le module activé.");
        Vector3 focusPosition = TargetObject.transform.position - mainCamera.transform.forward * focusDistance;
        StartCoroutine(MoveCamera(mainCamera.transform.position, focusPosition, TargetObject.transform.position));
    }

    void HandleRightClick()
    {
        if (isFocused)
        {
            isFocused = false;
            isMainObjectActive = true;

            BombRotation.IsModuleFocused = false; // Réactive la rotation de la bombe

            Debug.Log("Focus sur le module annulé.");
            ResetCameraToInitial();
        }
        else
        {
            if (isMainObjectActive)
            {
                Debug.Log("Clic droit : retour de l'objet principal à sa position initiale.");
                ResetMainObjectToInitialPosition();
            }
        }
    }

    void ResetCameraToInitial()
    {
        StartCoroutine(MoveCamera(mainCamera.transform.position, initialCameraPosition, mainCamera.transform.forward + initialCameraPosition));
    }

    void ResetMainObjectToInitialPosition()
    {
        // Logique pour ramener l'objet principal à sa position initiale (si nécessaire)
        Debug.Log("Objet principal ramené à la position initiale.");
        // Implémente ici le déplacement ou l'animation de l'objet principal.
    }

    System.Collections.IEnumerator MoveCamera(Vector3 startPosition, Vector3 endPosition, Vector3 lookAtPoint)
    {
        float elapsedTime = 0f;

        // Transition fluide
        while (elapsedTime < 1f)
        {
            elapsedTime += Time.deltaTime * transitionSpeed;

            // Interpolation de la position de la caméra
            mainCamera.transform.position = Vector3.Lerp(startPosition, endPosition, elapsedTime);

            // La caméra regarde le point cible
            mainCamera.transform.LookAt(lookAtPoint);
            yield return null;
        }

        // Assurer que la caméra finit au bon endroit
        mainCamera.transform.position = endPosition;
        mainCamera.transform.LookAt(lookAtPoint);
    }
}
