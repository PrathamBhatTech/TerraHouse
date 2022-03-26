# Terrahouse
 
Humanity is confronted with the grand challenge of how to increase agricultural production to achieve food security during the 21st century and feed a population that is expected to grow to 10 billion people. 

With food security becoming a larger problem, we need to innovate technologies that facilitate sustainable communities to grow their own produce. Doing this will aid communities and colonies become self-reliant and strengthen food security. This can be done on large scale in agragian parts of the country as shown in the Netherlands, but it also important to be able to implement these techniques in urban living spaces on a smaller scale. This decentralizes and democratizes the process of food production.

In this project, we aim to tackle the above problems.  We will provide several choices for a crop and based on that, our prototype will create and sustain a suitable environment for the chosen crop. We will also be implementing Machine Learning to receive/ record various factor of the crop and determine if the fruits are ripe. 

This has massive potential to be expanded into self sustainable communities to help create sustainable precision agricultural environment. 

Following is the hardware we will be using: 

1) Servo Motor - To open the window of the prototype in order to regulate temperature.

2) Soil Moisture detection - To detect moisture level of soil which will be regulated using the water pump.

3) Temperature Sensor - To detect temperature of the environment inside the prototype in order to regulate the temperature based on chosen crop

4) LDR - To detect Light and facilitate simulating sunlight: Light is one of the most important factors for plant growth and development, regulating plants’ photosynthesis, metabolism, morphogenesis, gene expression, and other physiological responses. Modifying the light wavelength, photon flux (quantity of light), and photoperiod lets one to adjust biomass accumulation, flowering time, stem elongation, and nutritional quality. 

5) Water Pump - To pump water through a pipe with holes that is placed on the soil(Drip irrigation) in order to regulate the soil moisture.

6) Humidity Sensor: To regulate/ analyse humidity inside the prototype

7) NodeMCU (ESP8266) - Used to collect data from the sensors and maintain the conditions 
     of the prototype
      - Data can be accessed from the website locally hosted on the NodeMCU and  its 
          functions can be manually controlled

      - Choice between multiple preset data of plant conditions for best growth for the type of 
       plant.

Software: 
-CNN model to detect ripe fruit and store various factors of crop growth. 
-Interface to chose the crop
