using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class ReadTwoArduinoValuesExample : MonoBehaviour
{
    private SerialPort serialPort;
    public string portName = "COM3";
    public int baudrate = 9600;

    // Déclaration d'un tableau destiné à recevoir les valeurs des capteurs arduino.
    public float[] values = {0,0};
    
    // Start is called before the first frame update
    void Start()
    {
        serialPort = new SerialPort(portName, baudrate);
        
        try
        {
            serialPort.Open();
        }
        catch
        {
            Debug.Log("Arduino not connected");
        }

        try
        {

            serialPort.ReadTimeout = 1;

        }
        catch
        {
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (serialPort.IsOpen)
        {
            try
            {
                // Lire les valeures de l'arduino par ligne. En séparant les valeurs à chaque virgule.
                string[] sensorValues = serialPort.ReadLine().Split(',');
                // convertir le text en chiffre decimale et le ranger dans la 1ere case du tableau de valeurs 
                values[0] = float.Parse(sensorValues[0]);
                // passer de 0 - 1023 à 0 - 1
                values[0] = values[0] / 1023;
                // valeur entre 0 et 2
                values[0] *= 2;
                // valeur entre -1 et 1
                values[0] -= 1;
                
                // convertir le text en chiffre decimale et le ranger dans la 2ere case du tableau de valeurs 
                values[1] = float.Parse(sensorValues[1]);
                // convertir le text en chiffre decimale et le ranger dans la 1ere case du tableau de valeurs 
                values[1] = float.Parse(sensorValues[1]);
                // passer de 0 - 1023 à 0 - 1
                values[1] = values[1] / 1023;
                // valeur entre 0 et 2
                values[1] *= 2;
                // valeur entre -1 et 1
                values[1] -= 1;
            }
            catch (System.Exception e)
            {
                
            }
            
        }
    }
    
    private void OnDisable()
    {
        serialPort.Close();
    }
}
