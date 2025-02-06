using System.Collections;
using UnityEngine;
using TMPro;

public class ButtonModule : ModuleState
{
    [SerializeField]
    private float holdTime = 2f;

    [SerializeField]
    private TextMeshPro digitsTimer;

    [SerializeField]
    private GameObject lightBand;

    [SerializeField]
    private Material lightBandIdleMaterial; // Matériau par défaut du LightBand

    [SerializeField]
    private Material lightBandActiveMaterial; // Matériau lorsque le bouton est maintenu

    private Coroutine lightBandCoroutine;

    private float holdTimer = 0f;
    private bool isHolding = false;

    // Audio pour le clic
    [SerializeField]
    private AudioClip clickSound; // Son du clic

    private void Update()
    {
        if (isHolding && !IsModuleCompleted && this.enabled)
        {
            holdTimer += Time.deltaTime;
        }
    }

    private void OnMouseDown()
    {
        if (IsModuleCompleted || !this.enabled) return;

        // Jouer le son du clic lorsque l'utilisateur appuie sur le bouton
        PlayClickSound();

        isHolding = true;
        holdTimer = 0f;

        // Activer la logique du LightBand
        if (lightBandCoroutine != null)
        {
            StopCoroutine(lightBandCoroutine);
        }

        lightBandCoroutine = StartCoroutine(ChangeLightBandMaterialAfterDelay(lightBandActiveMaterial, 1f));
    }

    private void OnMouseUp()
    {
        if (IsModuleCompleted || !this.enabled) return;

        isHolding = false;

        // Vérifier la condition de succès ou d'échec
        if (CheckSuccessCondition())
        {
            // Si le module n'est pas encore terminé, définir le succès
            if (!IsModuleCompleted)
            {
                SetSuccessState();
            }
        }
        else
        {
            // Si le module n'est pas encore terminé, définir l'erreur
            if (!IsModuleCompleted)
            {
                SetErrorState();
            }
        }

        holdTimer = 0f;

        // Réinitialiser le matériau du LightBand
        if (lightBandCoroutine != null)
        {
            StopCoroutine(lightBandCoroutine);
            lightBandCoroutine = null;
        }

        SetLightBandMaterial(lightBandIdleMaterial);
    }

    /// <summary>
    /// Change le matériau du LightBand après un délai.
    /// </summary>
    private IEnumerator ChangeLightBandMaterialAfterDelay(Material material, float delay)
    {
        yield return new WaitForSeconds(delay);

        if (isHolding)
        {
            SetLightBandMaterial(material);
        }
    }

    /// <summary>
    /// Applique le matériau au LightBand.
    /// </summary>
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

    /// <summary>
    /// Jouer le son de clic lorsque l'utilisateur interagit.
    /// </summary>
    private void PlayClickSound()
    {
        if (audioSource != null && clickSound != null)
        {
            audioSource.PlayOneShot(clickSound);
        }
    }

    public override bool CheckSuccessCondition()
    {
        return holdTimer >= holdTime && digitsTimer != null && digitsTimer.text.Contains("5");
    }
}
