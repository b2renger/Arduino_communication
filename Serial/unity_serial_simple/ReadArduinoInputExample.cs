using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class ReadArduinoInputExample : MonoBehaviour
{
    private SerialPort serialPort;
    public string portName = "COM3";
    public int baudrate = 9600;
    
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
                string sensorValue = serialPort.ReadLine();
                Debug.Log(sensorValue);
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
