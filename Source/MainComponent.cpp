/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"





MainContentComponent::MainContentComponent()
{
    setSize (450, 450);
    addAndMakeVisible(&flyComponent);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
   
}

void MainContentComponent::resized()
{
    flyComponent.setBounds(getLocalBounds());
}


