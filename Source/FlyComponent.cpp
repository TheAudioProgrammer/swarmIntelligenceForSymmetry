/*
  ==============================================================================

    FlyComponent.cpp
    Created: 5 Nov 2017 10:40:14am
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FlyComponent.h"


void FlyComponent::initializeFlies(std::vector <std::vector <int>> &theFlies)
{
    //temp vector to hold x and y positions for each fly
    std::vector <int> fly;
    
    //initialize fly positions
    for (int i = 0; i < population; i++)
    {
        //find random x and y vals and push these positions into a vector
        int posX = r.nextInt(getWidth());
        int posY = r.nextInt(getHeight());
        fly.push_back(posX);
        fly.push_back(posY);
        
        //push the vector pair into 
        theFlies.push_back (fly);
        
        //clear vector for next fly x/y positions
        fly.clear();
    }
}

//==============================================================================

void FlyComponent::drawFlies (std::vector <std::vector <int>> &theFlies, Graphics &g)
{
    //make a population of flies
    for (int i = 0; i < population; i++)
    {
        //draw the flies in the positions determined in void initializeFlies
        for (int j = 0; j < dimensions; j+=2)
        {
            g.setColour(Colours::ghostwhite);
            g.drawEllipse(theFlies[i][j], theFlies[i][j + 1], 5, 5, 5);
        }
    }
}

//==============================================================================


void FlyComponent::comparePixelValues (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses)
{
    //each time this is called we need to clear previous fitness scores, otherwise we just keep pushing more positions into the vector!
    theFitnesses.clear();
    
    //the fitness score for each fly
    float flyFitness;
    
    for (int i = 0; i < population; i++)
    {
        for (int j = 0; j < dimensions; j+=2)
        {
            //number of tests each fly will do to find symmetry
            for (int k = 0; k < numTests; k++)
            {
                
                //pick a random number
                float randX = r.nextInt(upperBounds / 2);
                float randY = r.nextInt(upperBounds / 2);
                
                
                //find a pixel to the left/up to test against corresponding pixel right/down
                float rxLeft = theFlies[i][j] - randX;
                float rxRight = theFlies[i][j] + randX;
                float ryUp = theFlies[i][j + 1] - randY;
                float ryDown = theFlies[i][j + 1] + randY;
                
                
                
                //wrap around if the test pixel is outside the bounds of the search space
                if (rxLeft < 0)
                {
                    rxLeft = getWidth() - randX;
                }
                
                if (rxLeft > getWidth())
                {
                    rxLeft = randX;
                }
                
                if (rxRight < 0)
                {
                    rxRight = getWidth() - randX;
                }
                
                if (rxRight > getWidth())
                {
                    rxRight = randX;
                }
                
                if (ryUp < 0)
                {
                    ryUp = getHeight() - randY;
                }
                
                if (ryUp > getHeight())
                {
                    ryUp = randY;
                }
                
                if (ryDown < 0)
                {
                    ryDown = getHeight() - randY;
                }
                
                if (ryDown > getHeight())
                {
                    ryDown = randY;
                }
                
                //get the pixel at the random locations selected above
                Colour pixColourLeft = glass.getPixelAt (rxLeft, ryUp);
                Colour pixColourRight = glass.getPixelAt (rxRight, ryDown);
                
                //find their brightness
                float pixelBrightnessLeft = pixColourLeft.getBrightness();
                float pixelBrightnessRight = pixColourRight.getBrightness();
                
                //compare the two pixel's brightness against each other and create a fitness score
                flyFitness = fabs(pixelBrightnessLeft - pixelBrightnessRight) + flyFitness;
               
                
            }
            
            //push the fitness scores into a vector
            theFitnesses.push_back(flyFitness);
            
            //clear fitness scores for next fly
            flyFitness = 0;
        }
    }
    
    //find the fittest fly- the lowest score out of all fly fitnesses
    auto result = std::min_element(std::begin(theFitnesses), std::end(theFitnesses));
    fittestInSwarm = (int)(result - theFitnesses.begin());
}

//==============================================================================

void FlyComponent::updateFlies (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses)
{
    //look at neighboring flies fitness scores and determine who's fitter (index of fly before and after)
    for (int i = 0; i < population; i++)
    {
        int leftFly;
        int rightFly;
        
        //if first fly, fly to the left is the last fly
        if (i == 0)
        {
            leftFly = (int) theFitnesses.size() - 1;
        }
        else
        {
            leftFly = i - 1;
        }
        
        //if last fly, fly to the right is the first fly
        if (i == theFitnesses.size() - 1)
        {
            rightFly = 0;
        } else
        {
            rightFly = i + 1;
        }
        
        int bestNeighbor = 0;
        
        //find best neighbor
        if (theFitnesses[leftFly] < theFitnesses[rightFly])
        {
            bestNeighbor = leftFly;
        }
        else
        {
            bestNeighbor = rightFly;
        }
        
        //choose a random number between 0 and 1
        float threshold = r.nextFloat();
        
        //if it's less than the disturbance threshold then randomly disperse this fly into data space to search for a better solution
        if (threshold < disturbanceThresh)
        {
            for (int j = 0; j < dimensions; j++)
            {
                theFlies[i][j] = r.nextInt(getWidth());
            }
        }
        else
        {
            //fitness function- move towards the best neighbor
            for (int j = 0; j < dimensions; j++)
            {
                //update position
                theFlies[i][j] = theFlies[bestNeighbor][j] + r.nextFloat() * (theFlies[fittestInSwarm][j] - theFlies[bestNeighbor][j]);
                
            }
        }
    }
}


//==============================================================================

FlyComponent::FlyComponent()
{
    
    setSize (450, 450);
    
    //can adjust framerate for performance
    setFramesPerSecond(10);
    
    //choose your own pic here
    glass = ImageCache::getFromFile (File("/Users/djatwar/Desktop/glass.jpg"));
    
    //2 dimensions- x and y
    dimensions = 2;
    
    //fly population
    population = 25;
    
    //define search space
    lowerBounds = 0;
    upperBounds = 450;
    
    //threshold to randomly disperse flies
    disturbanceThresh = 0.2;
    
    //number of times for each fly to check symmetry
    numTests = 125;
    
    //send flies randomly into search space
    initializeFlies(theFlies);
}

FlyComponent::~FlyComponent()
{
}

void FlyComponent::paint (Graphics& g)
{
    //draw photo
    g.drawImageAt(glass, 0, 0);
    
    //draw flies
    drawFlies(theFlies, g);
    
}

void FlyComponent::update()
{
    
    
    //compare random corresponding pixels left/right and up/down and compare to see if they're the same
    comparePixelValues(theFlies, theFitnesses);
    
    //update fly position or disperse based on fitness function
    updateFlies(theFlies, theFitnesses);
}


void FlyComponent::resized()
{
}
