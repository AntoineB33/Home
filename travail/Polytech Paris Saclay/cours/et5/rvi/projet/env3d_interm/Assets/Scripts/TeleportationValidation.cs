using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class TeleportationValidation : MonoBehaviour
{
    public XRRayInteractor rayInteractor; // Assign the XR Ray Interactor in the inspector
    public LayerMask teleportLayer;       // Set this to the floor layer
    public LayerMask obstacleLayer;       // Set this to objects that block teleportation
    public float headClearanceHeight = 2.0f; // Adjust based on player height

    private XRInteractorLineVisual lineVisual;

    void Start()
    {
        if (rayInteractor == null)
            rayInteractor = GetComponent<XRRayInteractor>();

        lineVisual = GetComponent<XRInteractorLineVisual>();
    }

    void Update()
    {
        ValidateTeleportation();
    }

    void ValidateTeleportation()
    {
        Vector3 hitPoint = Vector3.zero;
        Vector3 normal = Vector3.zero;
        int hitLayer = 0;
        bool isValidTarget = false;

        // Ensure that the XR Ray Interactor is hitting something
        if (rayInteractor.TryGetHitInfo(ref hitPoint, ref normal, ref hitLayer, ref isValidTarget))
        {
            // Check if the target is a valid teleport surface
            if (isValidTarget && ((1 << hitLayer) & teleportLayer) != 0)
            {
                // Perform an upward raycast from the teleportation target to check for obstacles
                if (Physics.Raycast(hitPoint, Vector3.up, headClearanceHeight, obstacleLayer))
                {
                    // Object detected above, disable teleportation
                    DisableTeleportMarker();
                }
                else
                {
                    // No obstacle detected, allow teleportation
                    EnableTeleportMarker();
                }
            }
        }
    }

    void DisableTeleportMarker()
    {
        if (lineVisual != null)
        {
            lineVisual.enabled = false; // Hide teleport ray
        }
    }

    void EnableTeleportMarker()
    {
        if (lineVisual != null)
        {
            lineVisual.enabled = true; // Show teleport ray
        }
    }
}
