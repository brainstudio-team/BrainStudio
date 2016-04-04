BrainStudio Tutorial Exercises
==============================

These exercises will take you through a tour of the basic set of BrainStudio
features. You will implement a few simple neural systems and study their
behaviour.

Sample solutions to these exercises are distributed with the BrainStudio
repository. Don't cheat! Look at the solutions only after trying for yourself.

[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex1: Single PING oscillator

## Introduction

Gamma oscillations are a very characteristic signature of cortical dynamics,
and are theorized to play a major role in multiple cognitive processes,
including attention, perception and working memory. In this series of exercises
we'll explore a few neural mechanisms that are capable of generating gamma
oscillations.

Pyramidal-InterNeuron Gamma (PING) oscillators are believed to be a major
example of such mechanisms. One of the simplest neural circuits of this kind
involves two populations of neurons, an excitatory and an inhibitory one.  Both
populations are strongly connected to themselves and weakly connected to the
other.

The basic dynamics work as follows: some incoming current makes some excitatory
neurons fire, which excites both other excitatory neurons and the inhibitory
ones. This, in turn, inhibits the excitatory population a bit later, shutting
down inhibition and letting the current act again. This creates an oscillation
with a frequency that depends on the parameters of the neurons and the
synapses, but is typically around 40Hz.

## What you'll learn

In this exercise you will use the basic features of BrainStudio, like adding
nodes and edges, choosing neuron models and parameter sets and running
simulations. You will also use BrainStudio's stimulation widget to inject
current into the populations and observe a range of emergent dynamical
phenomena.

## What to do

To complete this exercise, you should:

1. Create a population of 800 Izhikevich neurons, with the 'Excitatory'
parameter set and another population of 200 Izhikevich neurons with the
'Inhibitory' parameter set.
  * *Hint*: You can add nodes from the menu in the top left or with a double
  click anywhere in the main window. Choose first a neuron model and then one
  of the parameter sets available for that model, or write your own.

2. Add a connection from the excitatory to the inhibitory population, another
connection from the inhibitory to the excitatory, and a recurrent connection
within the inhibitory. Use a delay of 6ms for all synapses.
  * *Hint*: You can add synapses by clicking and dragging on the '+' button
  in each node. You can use formulas and functions inside the definition of
  the connections, including random number generators like `RANDF`. Try using
  a weight of `2*RANDF()` for the excitatory synapses and `-2*RANDF()` for the
  inhibitory ones.

3. Add a recurrent connection within the excitatory module with stronger
weights (distributed randomly between 0 and 17) and with a density of 1% (Try
changing the connectivity pattern when adding the synapse) and a delay of 6ms.

4. Load the network in NeMo and run it using the buttons in the toolbar. You
can use the 'Stimulation' button to start the stimulation widget and inject
current into the modules. You can explore different visualizations in the menu
at the bottom left.

Now you can experiment in real time with the stimulation (you can stimulate one
or both populations, with varying intensity) and the neuron parameters.  Under
the right conditions, you should see the excitatory group oscillate with a
frequency in the gamma band. Congratulations, that's your first PING
oscillator!

<u>Extra</u>: Try multiple configurations of stimulations and parameters. Can
you spot different dynamical regimes? What happens if you inject a current that
is also oscillating? How does the frequency of the stimulation affect the
behaviour of the population?


[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex2: Uncoupled PING pair

## Introduction

The complex oscillatory patterns we observe in cortical Local Field Potentials
(LFP) and EEG cannot be accounted for with one type of oscillator only. In
practice, we observe complicated power spectra with multiple frequencies. As
mentioned in exercise 1, changes in the behaviour of the PING can be modelled
with different parameters of the neurons and the connections.

The parameter that affects the frequency the most is the synaptic delay -- i.e.
how much does information take to go back and forth between the populations. In
this exercise you'll experiment with multiple realizations of this PING set-up
and how it is affected (and limited) by the underlying assumptions of the
model.

## What you'll learn

In this exercise you'll learn about other features and shortcuts to make
network design more convenient. You will also look into more visualization
tools available in BrainStudio to perform some exploratory analysis of your
simulation.

## What to do

To complete this exercise, you should:

1. Start from your previous PING brn file.
    * *Hint*: Look at the menus in the top left and use the keyboard shortcuts
    to open, load and save brn files.

2. Make a copy of both modules and replicate the connections between them,
this time with a delay of 4ms in all synapses.
    * *Hint*: You can use BrainStudio's Copy+Paste shortcuts to save you time.
    Remember to rename the modules to avoid conflicts.

3. Run the network as you did with the single PING. Remember to inject current
in both PING groups. How do they react to current? Do they need the same or
different volumes of current to operate?

4. Do right click on any of the nodes and select 'New plot'. Inspect the options
in the window that just appeared in the bottom right. You can drag the window
around and select various visualizations.
    * *Hint*: Hover your mouse over any element to obtain a short description.

Now you should observe both PING groups oscillating simultaneously. What is the
difference between the two? Select 'Raster plot' in the visualization menu in
the bottom left and drag the excitatory modules in the screen so that one is
right above the other. You should see that they have different frequencies, and
they sometimes fire at the same time and sometimes they don't -- we say these
oscillators are *incoherent*.

<u>Extra</u>: You can pause the simulation and try to estimate the intrinsic
frequency of each oscillator. Now that you're a PING expert, you can try to
make another PING with yet another delay and see how high or low the frequency
of this kind of oscillators can get. This will give you an idea of the
limitations of such a simple model.

This was hopefully an easy one. Leave the tab open and move on to the next
exercise.


[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex3: Coupled PING pair

## Introduction

A leading theory of how different neural groups communicate amidst a sea of
uncontrolled oscillations is Pascal Fries' *Communication Through Coherence*
(CTC) hypothesis. CTC states that neural populations can communicate with each
other when they are both oscillating in a coherent manner and when they are
oscillating *synchronously* -- both have peaks and lows at the same time. This
means that both populations are more receptive in the periods when the other
population is more active, enabling communication.

In this exercise we will implement a toy example of CTC within our PING setting
and explain how coherence can be generated in a biologically realistic way. We
will also attempt to quantify (in a *very* simplistic way) the "communication"
between these groups.

## What you'll learn

In this exercise you will learn about one of the key features of BrainStudio --
real-time effective connectivity estimation. Anatomical connectivity (actual
synapses and connections) determine the behaviour of the system, but it is not
always clear how they relate to the system's function. Using BrainStudio's
effective connectivity estimators you can explore what populations are actually
related to each other and how, determining functional and causal relations
between them.

## What to do

To complete this exercise, you should:

1. Start from your previous uncoupled PING pair brn file.

2. Add excitatory synapses from each excitatory module to the inhibitory module
of the other PING. These should be sparse (1% density), have reasonably strong
weights (from 0 to 15) and a moderate delay (5ms).

3. Run the network and add a small current (~5pA) to both excitatory
populations. As before, you can move the blocks around and use various
visualizations. You should see both groups oscillating, but unlike the previous
case they should be coordinated and at the same frequency -- we say they are
synchronized.

4. Tick the 'Net Analysis' checkbox in the visualization menu in the bottom
left. A drop-down menu just on the right will show available effective
connectivity measures. Try them out in this example and in the uncoupled
PING pair. What do these measures say about the interaction between the
groups?

If everything went well, you should have seen strong effective connectivity
(i.e. darker lines) between both excitatory and both inhibitory modules in the
coupled case and weaker connectivity (i.e. lighter lines) in the uncoupled
case. This demonstrates that distant clusters of neurons can share information
and be strongly correlated despite not having any direct connection between
them.

Neuroscientifically, this example suggests that long-range excitation of other
inhibitory populations can generate long-range coherence and synchrony between
excitatory populations. It also provides a tiny contribution to our argument
that CTC might actually be a thing in the brain. Of course, years of research
have taken it much further than this!


[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex4: Multistable network

## Introduction

Oscillations are not the only interesting neural dynamics we find in the
cortex. Equally ubiquitous is the phenomenon of multistability -- in short, a
set of excitatory groups that compete against each other in a winner-take-all
fashion. In this exercise, you will implement and run a simple bistable network
(i.e. a multistable set-up with two competing nodes).

## What you'll learn

In this exercise you will create more sophisticated connectivity patterns and
make use of more BrainStudio's built-in functions. You will also learn about
the structure of brn files and modify one to change the fine details of the
simulation. Keep an eye on the brn file as you go and try to infer how the brn
reflects the structure of the network you see in the main window. The specs
will be less specific this time so you will have to find your way to observe
the phenomenon you are looking for.

## What to do

To complete this exercise, you should:

1. Start a new brn file from scratch (you can use the shortcut `Ctrl+N`). Add
two excitatory populations of 400 neurons each and an inhibitory population of
200 neurons, with the same model and parameters as in the previous exercises.

2. Add dense inhibitory connections from the inhibitory module to the whole
network.

3. Add strong, delayed recurrent connections within each excitatory module.
    * *Hint*: You might want to try random delays. Since delays are integers,
    you can use the function `RANDI(low, high)` in the definition of the edge.

4. Run the simulation and experiment with the stimulation. What phenomena do
you observe? Try changing the intra- and inter-module connectivity settings.
    * *Hint*: you can modify the network directly from the brn file editor
    in the right. When you are done, press the green arrow button and your
    network will be ploted in the main window.

This system should have two different dynamical regimes that depend on the
relative strength of the intra- and inter-module connections. When intra-module
connections are much stronger than inter-module ones, both modules compete
through the common inhibitory pool so that when one is active the other is
quiet, and who is in control changes spontaneously. When inter-module
connections are close in strength to the intra-module ones, there is no real
distinction between the modules and the whole system starts oscillating like a
single PING group.

Under some conditions, you can also see *theta-modulated gamma* oscillations
(or theta envelopes), i.e. a base oscillation with gamma frequency and an
amplitude determined by a slower theta frequency.

And that's it! You should be a BrainStudio pro by now. In the next set of
exercises we will cover how to create custom nodes and edges and more advanced
features.

