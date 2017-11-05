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
        fly.clear();
    }
}

//==============================================================================

void FlyComponent::drawFlies (std::vector <std::vector <int>> &theFlies, Graphics &g)
{
    //draw flies
    for (int i = 0; i < population; i++)
    {
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
    
    for (int i = 0; i < population; i++)
    {
        for (int j = 0; j < dimensions; j+=2)
        {
            for (int k = 0; k < 1000; k++)
            {
                float randX = r.nextInt(upperBounds / 2);
                float randY = r.nextInt(upperBounds / 2);
                
                
                
                float rxLeft = theFlies[i][j] - randX;
                float rxRight = theFlies[i][j] + randX;
                float ryUp = theFlies[i][j + 1] - randY;
                float ryDown = theFlies[i][j + 1] + randY;
                
                //std::cout << "Random X: " << randX << " " << "Random Y: " << randY << std::endl;
                
                
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
                
                
                //                std::cout << "Fly position X: " << theFlies[i][j] << " Left pixel: " << rxLeft << " Right pixel: " << rxRight << std::endl;
                //
                //                std::cout << "Fly position Y: " << theFlies[i][j + 1] << " Left pixel: " << ryUp << " Right pixel: " << ryDown << std::endl;
                
                
                //go to the pixels
                Colour pixColourLeft = glass.getPixelAt (rxLeft, ryUp);
                Colour pixColourRight = glass.getPixelAt (rxRight, ryDown);
                
                //find their brightness
                float pixelBrightnessLeft = pixColourLeft.getBrightness();
                float pixelBrightnessRight = pixColourRight.getBrightness();
                
                
                //std::cout << "Brightness Left: " << pixelBrightnessLeft << " Brightness Right: "  << pixelBrightnessRight << std::endl;
                
                
                flyFitness = fabs(pixelBrightnessLeft - pixelBrightnessRight) + flyFitness;
                //std::cout << flyFitness << std::endl;
                
            }
            
            //std::cout << "Fly: " << i << " Fly Fitness: " << flyFitness << std::endl;
            theFitnesses.push_back(flyFitness);
            flyFitness = 0;
        }
    }
    //std::cout << theFitnesses.size() << std::endl;
    auto result = std::min_element(std::begin(theFitnesses), std::end(theFitnesses));
    fittestInSwarm = (int)(result - theFitnesses.begin());
    //std::cout << fittestInSwarm << std::endl;
}

//==============================================================================

void FlyComponent::updateFlies (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses)
{
    for (int i = 0; i < population; i++)
    {
        int leftFly;
        int rightFly;
        
        if (i == 0)
        {
            leftFly = (int) theFitnesses.size() - 1;
        }
        else
        {
            leftFly = i - 1;
        }
        
        if (i == theFitnesses.size() - 1)
        {
            rightFly = 0;
        } else
        {
            rightFly = i + 1;
        }
        
        int bestNeighbor = 0;
        
        if (theFitnesses[leftFly] < theFitnesses[rightFly])
        {
            bestNeighbor = leftFly;
        }
        else
        {
            bestNeighbor = rightFly;
        }
        
        
        float threshold = r.nextFloat();
        
        if (threshold < disturbanceThresh)
        {
            for (int j = 0; j < dimensions; j++)
            {
                theFlies[i][j] = r.nextInt(getWidth());
            }
        }
        else
        {
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
    setFramesPerSecond(10);
    
    
    glass = ImageCache::getFromFile (File("/Users/djatwar/Desktop/glass.jpg"));
    
    //2 dimensions- x and y
    dimensions = 2;
    population = 25;
    
    lowerBounds = 0;
    upperBounds = 450;
    
    disturbanceThresh = 0.2;
    
    initializeFlies(theFlies);
    
}

FlyComponent::~FlyComponent()
{
}

void FlyComponent::paint (Graphics& g)
{
    g.drawImageAt(glass, 0, 0);
    drawFlies(theFlies, g);
    
}

void FlyComponent::update()
{
    theFitnesses.clear();
    comparePixelValues(theFlies, theFitnesses);
    updateFlies(theFlies, theFitnesses);
}


void FlyComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
