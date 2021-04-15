using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyPlayerControlledByTwoArduinoValues : MonoBehaviour
{
    // Cette variable fait le lien avec le script ReadArduinoInputExample.
    // Il faut lui drag'n'drop l'objet ReadArduino dans l'editeur.
    public ReadTwoArduinoValuesExample myArduino;

    // Update is called once per frame
    void Update()
    {
        // Lecture de la 1er valeur
        Debug.Log("value 1 : "+myArduino.values[0]);
        
        // Lecture de la 2eme valeur
        Debug.Log("value 2 : "+myArduino.values[1]);
    }
}
