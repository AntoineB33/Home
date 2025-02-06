using UnityEngine;
using Unity.XR.PXR;

public class HighlightOnHover : MonoBehaviour
{
    private bool isHovered = false;
    private bool isProcessing = false;
    [SerializeField]
    GameObject TargetObject;

    [SerializeField]
    private int width = 4;

    private Outline Outline;

    void Start()
    {
        // V�rifie si le composant Outline existe d�j�, sinon ajoute-le
        Outline = TargetObject.GetComponent<Outline>();
        if (Outline == null)
        {
            Outline = TargetObject.AddComponent<Outline>();
        }
        Outline.enabled = false; // Assure-toi qu'il est d�sactiv� au d�part
    }
    void Update()
    {
        // V�rifie si la souris est sur l'objet
        if (IsMouseOver())
        {
            if (!isHovered)
            {
                // L'objet a �t� survol� pour la premi�re fois
                OnMouseEnter();
            }
        }
        else
        {
            if (isHovered)
            {
                // La souris est partie de l'objet
                OnMouseExit();
            }
        }

        
    }

    bool IsMouseOver()
    {
        // Si un raycast de la souris touche l'objet
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
        // D�marre le traitement lorsque la souris passe sur l'objet
        isHovered = true;
        if (!isProcessing)
        {
            HighlightBorders();
        }
    }

    void OnMouseExit()
    {
        // Arr�te le traitement lorsque la souris quitte l'objet
        isHovered = false;
        UnhighlightBorders();
    }

    void HighlightBorders()
    {
        // Place ici ton code pour d�marrer le traitement
        isProcessing = true;
        // Debug.Log("Traitement d�marr�"); 
        
        Outline.enabled = true;
        Outline.OutlineColor = Color.red;
        Outline.OutlineWidth = width;
        Outline.OutlineMode = Outline.Mode.OutlineAll;
    }

    void UnhighlightBorders()
    {
        // Place ici ton code pour arr�ter le traitement
        isProcessing = false;
        // Debug.Log("Traitement arr�t�");

        Outline.enabled = false;
    }
}

