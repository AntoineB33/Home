using UnityEngine;
using TMPro;

public class TimerLogic : MonoBehaviour
{
    [SerializeField]
    TextMeshPro TimerText;

    [SerializeField]
    private AudioClip TickSound;

    private AudioSource AudioSource;

    float RemainingTime = 10f;
    private int lastSecond = -1; // Pour éviter de jouer le son plusieurs fois par seconde

    // Start is called before the first frame update
    void Start() {
        if (TimerText == null)
        {
            Debug.LogError("TextMeshPro non assigné !");
            return;
        }

        AudioSource = GetComponent<AudioSource>();
        if (AudioSource == null)
        {
            Debug.LogError("AudioSource manquant !");
        }
    }

    // Update is called once per frame
    void Update() {
        if (RemainingTime > 0)
        {
            RemainingTime -= Time.deltaTime;
        }
        else if (RemainingTime <= 0)
        {
            RemainingTime = 0;
            TimerText.text = "BOOM!";
            //Explosion de la bombe()
        }

        int minutes = Mathf.FloorToInt(RemainingTime / 60);
        int seconds = Mathf.FloorToInt(RemainingTime % 60);
        TimerText.text = string.Format("{0:00}:{1:00}", minutes, seconds);

        if (seconds != lastSecond)
        {
            lastSecond = seconds;

            if (AudioSource != null && TickSound != null)
            {
                AudioSource.PlayOneShot(TickSound);
            }
        }
    }
}
