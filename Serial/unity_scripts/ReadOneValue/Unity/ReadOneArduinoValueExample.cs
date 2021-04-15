using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class ReadOneArduinoValueExample : MonoBehaviour
{
    private SerialPort serialPort;
    public string portName = "COM3";
    public int baudrate = 9600;

    public float value = 0;
    
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
                string sensorValue = serialPort.ReadLine();
                // convertir le text en chiffre decimale et le ranger dans la 1ere case du tableau de valeurs 
                value = float.Parse(sensorValue);
                // passer la valeur de 0 - 1023 à 0 - 1
                value = value / 1023;
                // valeur entre 0 et 2
                value *= 2;
                // valeur entre -1 et 1
                value -= 1;
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
