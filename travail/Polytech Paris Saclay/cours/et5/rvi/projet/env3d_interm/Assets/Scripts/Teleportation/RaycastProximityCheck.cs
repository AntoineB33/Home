using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class RaycastProximityCheck : MonoBehaviour
{
    public float proximityThreshold = 3f;  // Adjust the proximity value
    public LayerMask collisionLayer;

    private XRRayInteractor rayInteractor;

    void Start()
    {
        rayInteractor = GetComponent<XRRayInteractor>();
    }

    void Update()
    {
        RaycastHit hitInfo;

        // Access the ray's origin directly from the transform of the XRRayInteractor
        Vector3 rayOrigin = rayInteractor.transform.position;
        Vector3 rayDirection = rayInteractor.transform.forward;

        Debug.Log("Raycast" + Physics.Raycast(rayOrigin, rayDirection, out hitInfo, rayInteractor.maxRaycastDistance, collisionLayer));
        Debug.Log("Time : " + Time.time);

        // Perform a raycast to check if we are close to a collider
        if (Physics.Raycast(rayOrigin, rayDirection, out hitInfo, rayInteractor.maxRaycastDistance, collisionLayer))
        {
            float distanceToCollider = Vector3.Distance(rayOrigin, hitInfo.point);
            if (distanceToCollider < proximityThreshold)
            {
                // Change color or behavior if too close
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
