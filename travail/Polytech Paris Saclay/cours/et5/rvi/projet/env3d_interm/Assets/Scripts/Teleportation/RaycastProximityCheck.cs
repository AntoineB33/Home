using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class RaycastProximityCheck : MonoBehaviour
{
    public float proximityThreshold = 0.2f;  // Adjust the proximity value
    public LayerMask collisionLayer;

    private XRRayInteractor rayInteractor;

    void Start()
    {
        rayInteractor = GetComponent<XRRayInteractor>();
    }

    void Update()
    {
        RaycastHit hitInfo;

        // Raycast to check the collider we're aiming at
        if (Physics.Raycast(rayInteractor.raycastOrigin.position, rayInteractor.raycastOrigin.forward, out hitInfo, rayInteractor.maxRaycastDistance, collisionLayer))
        {
            float distanceToCollider = Vector3.Distance(rayInteractor.raycastOrigin.position, hitInfo.point);
            if (distanceToCollider < proximityThreshold)
            {
                // Change color or behavior if too close
                // e.g., turn ray red
                rayInteractor.GetComponent<LineRenderer>().startColor = Color.red;
                rayInteractor.GetComponent<LineRenderer>().endColor = Color.red;
            }
            else
            {
                // Reset to normal color if not within the proximity threshold
                rayInteractor.GetComponent<LineRenderer>().startColor = Color.green;
                rayInteractor.GetComponent<LineRenderer>().endColor = Color.green;
            }
        }
        else
        {
            // Reset to normal color if nothing is hit
            rayInteractor.GetComponent<LineRenderer>().startColor = Color.green;
            rayInteractor.GetComponent<LineRenderer>().endColor = Color.green;
        }
    }
}
