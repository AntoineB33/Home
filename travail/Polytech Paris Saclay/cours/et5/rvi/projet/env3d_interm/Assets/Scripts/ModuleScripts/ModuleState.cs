using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class ModuleState : MonoBehaviour
{
    [SerializeField]
    protected Material successMaterial;

    [SerializeField]
    protected Material errorMaterial;

    [SerializeField]
    protected Material initialMaterial;

    [SerializeField]
    protected GameObject stateLED;

    [SerializeField]
    protected AudioSource audioSource;

    [SerializeField]
    protected AudioClip successSound;

    [SerializeField]
    protected AudioClip errorSound;

    [SerializeField]
    protected HighlightOnHover highlightOnHover;

    protected bool isSuccess = false;
    protected bool isError = false;

    /// <summary>
    /// Indique si le module est terminé (succès ou échec).
    /// </summary>
    public bool IsModuleCompleted => isSuccess || isError;

    /// <summary>
    /// Vérifie si la condition de succès est remplie. À implémenter dans les modules dérivés.
    /// </summary>
    public abstract bool CheckSuccessCondition();

    /// <summary>
    /// Définit l'état de succès du module.
    /// </summary>
    public void SetSuccessState()
    {
        if (IsModuleCompleted) return;

        isSuccess = true;
        if (stateLED != null)
        {
            SetMaterial(stateLED, successMaterial);
        }
        PlaySound(successSound);
        DisableInteractions();
    }

    /// <summary>
    /// Définit l'état d'erreur du module.
    /// </summary>
    public void SetErrorState()
    {
        if (IsModuleCompleted) return;

        isError = true;
        if (stateLED != null)
        {
            SetMaterial(stateLED, errorMaterial);
            StartCoroutine(ResetMaterialAfterDelay(stateLED, initialMaterial, 1f));
        }
        PlaySound(errorSound);
    }

    /// <summary>
    /// Applique un matériau à un GameObject.
    /// </summary>
    protected void SetMaterial(GameObject target, Material material)
    {
        MeshRenderer renderer = target.GetComponent<MeshRenderer>();
        if (renderer != null)
        {
            renderer.material = material;
        }
    }

    /// <summary>
    /// Joue un son à l'aide de l'AudioSource.
    /// </summary>
    protected void PlaySound(AudioClip clip)
    {
        if (audioSource != null && clip != null)
        {
            audioSource.PlayOneShot(clip);
        }
    }

    /// <summary>
    /// Réinitialise le matériau après un délai.
    /// </summary>
    protected IEnumerator ResetMaterialAfterDelay(GameObject target, Material material, float delay)
    {
        yield return new WaitForSeconds(delay);
        if (!isSuccess)
        {
            SetMaterial(target, material);
        }
    }

    /// <summary>
    /// Désactive les interactions pour ce module.
    /// </summary>
    protected void DisableInteractions()
    {
        if (highlightOnHover != null)
        {
            highlightOnHover.enabled = false;
        }
        Collider collider = GetComponent<Collider>();
        if (collider != null)
        {
            collider.enabled = false;
        }
        this.enabled = false;
    }
}
