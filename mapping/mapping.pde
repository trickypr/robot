import java.util.Arrays;
import java.util.Scanner;
import java.util.ArrayList;
import processing.serial.*;

//--------------------------------------------------------------------------------------//
//                                Program configuration                                 //
//--------------------------------------------------------------------------------------//
// Change these variables to change how the program functions

// When this is set to true, the program will atempt to join different maps. Small errors
// in the robot's movement and data accuracy may destroy these maps, so they are disabled
// by default
boolean shouldJoinMaps = false;

// The size of the window. Note that if you change this, you also have to change the `size`
// function in setup()
int windowSize = 999;

// The title of the window that will display the map
String windowTitle = "Robot mapper";

// The colors that will be used when rendering the window. These are values between 0 and 255
int backgroundColor = 240;
int drawColor = 000;

// This is the index of the port that the robot appears on. It may vary from computer to 
// computer. The port name is not hardcoded as the OS changes it between prorgram reboots
// This index starts at 1, not 0
int portIndex = 4;

//                                 Magic math variables                                 //
// Variables that perform static modifications to the variables defined above

// Do not change, modify the line above
int halfWindowSize = windowSize / 2;

//--------------------------------------------------------------------------------------//
//                                  Runtime variables                                   //
//--------------------------------------------------------------------------------------//

// Variable to store the serial port contents
Serial robotPort;

// All of the items on the map that have to be rendered.
ArrayList mapItems = new ArrayList();

// These are the distances tht we are going towards. These are used for combining maps.
float goingToDistance = 0;
float goingToAngle = 0.0f;
PVector lastPoint = new PVector(0,0);

//--------------------------------------------------------------------------------------//
//                                   Setup functions                                    //
//--------------------------------------------------------------------------------------//

// Main setup function
void setup() {
    // Initialise the window size and title
    size(999, 999);
    surface.setTitle(windowTitle);
    
    //
    // Serial setup
    //
    println("Available serial ports:");
    println(Serial.list());
    println("If you observe any errors connecting to the serial port, identify the serial port name,");
    println("count the number of items to it, and change the 'portIndex' variable in the main file");
    println();
    
    // Get the serial port that we will be connecting to. Print it to the console for debugging purposes
    String portName = Serial.list()[portIndex - 1];
    println("Using port:", portName);
    
    // Create a serial port instance
    robotPort = new Serial(this, portName, 9600);
    // Tell the port to hold everything in its buffer until a newline character
    robotPort.bufferUntil(10);
}

//--------------------------------------------------------------------------------------//
//                                  Handle serial port                                  //
//--------------------------------------------------------------------------------------//
void serialEvent(Serial port) {
    try {
        // If the serial port doesn't have any data, return
        if (robotPort.available() == 0) return;
        
        // Grab the contents of the port
        String console = port.readString();
        
        // Split it along the space characters
        String[] split = console.split("\\s+");
        
        // If the first item contains distance, then proces it
        if (split[0].contains("tan")) {
            // Create an arraylist to store all of the distances in
            ArrayList distances = new ArrayList();
            
            // Select only the distances from the array
            String[] dists = Arrays.copyOfRange(split, 1, split.length);
            println(Arrays.toString(dists));
            
            for (String dist : dists) {
                // Create a scanner to conver the distance to a float
                Scanner scanner = new Scanner(dist);
                
                float number = 0;
                
                if (scanner.hasNextInt()) {
                    number = (float)scanner.nextInt();
                } else if (scanner.hasNextDouble()) {
                    number = (float)scanner.nextDouble();
                }
                
                if (number == 0) {
                    number = 200;
                }
                
                distances.add(number);
            }
            
            if (shouldJoinMaps) {
                // Get the direction of movement of this point
                PVector movement = PVector.fromAngle(goingToAngle);
                //Set itsmagnatude based on the information provided
                movement.setMag(goingToDistance - (float)distances.get(0));
                
                // Move the last point by that much
                PVector coordernates = lastPoint.add(movement);
                
                if (mapItems.size() == 0) {
                    // If this is the first entry into the map list, it shouldn't have any translation
                    mapItems.add(new MapItem(new PVector(0,0), distances));
                } else{
                    // Otherwise we have to add the possition generated above
                    mapItems.add(new MapItem(coordernates, distances));
                    // Update the lastPoint to be correct
                    lastPoint = coordernates;
                }
                
                goingToDistance = (float)distances.get(getIndexOfLargest(distances));
                goingToAngle = (float)angleToPoint(getIndexOfLargest(distances), distances.size());
            } else {
                mapItems.clear();
                mapItems.add(new MapItem(new PVector(0,0), distances));
            }
            
            println(getIndexOfLargest(distances));
            robotPort.write(getIndexOfLargest(distances));
        }
        
        
    } catch(RuntimeException e) {
        // Catch any errors that occur and report them. By default these go unhandled
        e.printStackTrace();
    }
}


void draw() {
    // Clear thecontents of the window
    background(backgroundColor);
    
    ellipseMode(CENTER);
    fill(drawColor);
    
    for (int i =0; i < mapItems.size(); i++) {
        MapItem item = (MapItem)mapItems.get(i);
        circle(halfWindowSize + item.pos.x, halfWindowSize + item.pos.y, 10);
        item.draw(halfWindowSize);
    }
}
                    