using UnityEngine;

public class BombRotation : MonoBehaviour
{
    public float PCRotationSpeed = 10f; // Vitesse de rotation pour PC
    public Camera Cam;                 // Caméra utilisée pour définir l'axe de rotation

    // Variable statique pour vérifier si un module est focus
    public static bool IsModuleFocused = false;

    private void OnMouseDrag()
    {
        // Empêche la rotation si un module est focus
        if (IsModuleFocused) return;

        float RotX = Input.GetAxis("Mouse X") * PCRotationSpeed;
        float RotY = Input.GetAxis("Mouse Y") * PCRotationSpeed;

        Vector3 right = Vector3.Cross(Cam.transform.up, transform.position - Cam.transform.position);
        Vector3 up = Vector3.Cross(transform.position - Cam.transform.position, right);

        transform.rotation = Quaternion.AngleAxis(-RotX, up) * transform.rotation;
        transform.rotation = Quaternion.AngleAxis(RotY, right) * transform.rotation;
    }
}
