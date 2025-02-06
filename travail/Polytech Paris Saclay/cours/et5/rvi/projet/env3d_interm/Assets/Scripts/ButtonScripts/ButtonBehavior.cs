using System.Collections;
using TMPro;
using UnityEngine;

public class ButtonBehavior : MonoBehaviour
{
    [SerializeField]
    private float holdTime = 2f; // Temps pour maintenir le bouton avant de changer le matériau

    [SerializeField]
    private TextMeshPro digitsTimer;

    [SerializeField]
    private TextMeshPro error1;

    [SerializeField]
    private TextMeshPro error2;

    [SerializeField]
    public AudioSource audioSource;

    [SerializeField]
    public AudioClip soundButtonPressed;

    [SerializeField]
    public AudioClip soundErrorModule;

    [SerializeField] 
    public AudioClip soundSuccessModule;

    [SerializeField]
    public Material successModuleMaterial; // Matériau pour le succès

    [SerializeField]
    public Material errorModuleMaterial; // Matériau pour les erreurs

    [SerializeField]
    public Material lightBandMaterial; // Matériau pour l'état "en attente"

    [SerializeField]
    public Material initialGlassMaterial; // Matériau initial pour la LED et le lightBand

    [SerializeField]
    public GameObject lightBand;

    [SerializeField]
    public GameObject stateModuleLED;

    [SerializeField]
    private HighlightOnHover highlightOnHover;


    private bool isHolding = false; // Indique si le bouton est maintenu
    private float holdTimer = 0f; // Minuteur pour le maintien du bouton
    private Coroutine lightBandCoroutine = null; // Référence à la coroutine de changement de matériau
    private bool moduleSuccess = false; // Indique si le module a été réussi

    void Update()
    {
        if (isHolding)
        {
            holdTimer += Time.deltaTime;
        }
    }

    private void OnMouseDown()
    {
        if (!this.enabled) return;


        // Jouer le son lorsque le bouton est appuyé
        if (audioSource != null && soundButtonPressed != null)
        {
            audioSource.PlayOneShot(soundButtonPressed);
        }

        // Commencer à tenir le bouton
        isHolding = true;
        holdTimer = 0f;

        // Démarrer une coroutine pour changer le matériau après 1 seconde
        if (lightBandCoroutine != null)
        {
            StopCoroutine(lightBandCoroutine);
        }
        lightBandCoroutine = StartCoroutine(ChangeLightBandMaterialAfterDelay(lightBandMaterial, 1f));
    }

    private void OnMouseUp()
    {
        if (moduleSuccess)
        {
            // Ne rien réinitialiser si le module a déjà été réussi
            return;
        }

        // Réinitialiser immédiatement le matériau du lightBand
        if (lightBandCoroutine != null)
        {
            StopCoroutine(lightBandCoroutine);
            lightBandCoroutine = null;
        }
        SetLightBandMaterial(initialGlassMaterial);

        MeshRenderer stateModuleLEDMesh = stateModuleLED.GetComponent<MeshRenderer>();


        //Module passé avec succès
        if (holdTimer >= holdTime && digitsTimer.text.Contains("5"))
        {
            // Succès : changer les matériaux de la LED
            moduleSuccess = true; // Indiquer que le module a été réussi
            if (stateModuleLEDMesh != null)
            {
                stateModuleLEDMesh.material = successModuleMaterial;
            }

            audioSource.PlayOneShot(soundSuccessModule);

            DisableModule();

            // Laisser le matériau de succès appliqué sans le réinitialiser
        }
        else
        {
            // Erreur : gestion des erreurs (mise à jour des couleurs et du matériau)
            if (error1.color != Color.red)
            {
                error1.color = Color.red;
                audioSource.PlayOneShot(soundErrorModule);
            }
            else if (error2.color != Color.red)
            {
                error2.color = Color.red;
                audioSource.PlayOneShot(soundErrorModule);
            }
            else
            {
                // Explosion ou autre action critique à définir
            }

            // Mettre à jour le matériau de la LED pour signaler une erreur
            if (stateModuleLEDMesh != null)
            {
                stateModuleLEDMesh.material = errorModuleMaterial;

                // Revenir au matériau initial après une seconde
                StartCoroutine(ResetMaterialAfterDelay(stateModuleLED, initialGlassMaterial, 1f));
            }
        }

        // Réinitialiser les états
        isHolding = false;
        holdTimer = 0f;
    }

    // Méthode pour changer le matériau du lightBand
    private void SetLightBandMaterial(Material material)
    {
        if (lightBand != null)
        {
            MeshRenderer renderer = lightBand.GetComponent<MeshRenderer>();
            if (renderer != null)
            {
                renderer.material = material;
            }
        }
    }

    // Coroutine pour changer le matériau du lightBand après un délai
    private IEnumerator ChangeLightBandMaterialAfterDelay(Material material, float delay)
    {
        yield return new WaitForSeconds(delay);

        // Appliquer le matériau si le bouton est toujours appuyé et que le module n'a pas encore été réussi
        if (isHolding && !moduleSuccess)
        {
            SetLightBandMaterial(material);
        }
    }

    // Coroutine pour réinitialiser le matériau après un délai
    private IEnumerator ResetMaterialAfterDelay(GameObject targetObject, Material material, float delay)
    {
        yield return new WaitForSeconds(delay);

        if (!moduleSuccess) // Ne réinitialise pas si le module a été réussi
        {
            MeshRenderer renderer = targetObject.GetComponent<MeshRenderer>();
            if (renderer != null)
            {
                renderer.material = material;
            }
        }
    }

    private void DisableModule()
    {
        if (highlightOnHover != null)
        {
            highlightOnHover.enabled = false;
        }

        // Désactiver le collider pour bloquer les clics et survols
        Collider collider = GetComponent<Collider>();
        if (collider != null)
        {
            collider.enabled = false;
        }

        // Désactiver ce script
        this.enabled = false;
    }

}
