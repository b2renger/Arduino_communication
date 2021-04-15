using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyPlayerControlledByOneArduinoValue : MonoBehaviour
{
    // Cette variable fait le lien avec le script ReadArduinoInputExample.
    // Il faut lui drag'n'drop le GameObjet Arduino dans l'editeur.
    public ReadOneArduinoValueExample myArduino;

    // Update is called once per frame
    void Update()
    {
        // Lecture de la valeur
        Debug.Log("value : "+myArduino.value);
    }
}
