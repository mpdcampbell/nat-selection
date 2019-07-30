# nat-selection
This is a recreation of the natural selection simulation demonstrated in a [youtube video](https://www.youtube.com/watch?v=0ZGbIKd0XrM) by Primer. Primer has his own [github repository](https://github.com/Helpsypoo/primer), but none of his code was used here. The idea was to copy the simulation "rules" set out in the video and independently recreate it in C++. The code does not animate the simulation but uses gnuplot to plot how the population number and traits changed.

#### **Simulation Rules** 
The simulation creates a squared grid "map", with "blob" creatures living at the edges, and randomly populates it with "food". Each day the blob creatures leave home to hunt for food. If a blob eats one food object and makes it home, it will survive to the next day. If a blob eats two "food" and makes it home then they reproduce asexually. Each blob has three traits to help it hunt food:
 - Size - A blob can eat another blob so long as the prey is 20% smaller than it.
 - Speed - The number of grid spaces or "steps" it can take at once.
 - Sense - The radius of grid spaces around it that a blob can see. 
   
However, each of these traits has an energy cost. A blob starts each day with an equal amount of energy and each step taken has an energy cost, where *cost = size<sup>3</sup>speed<sup>2</sup>+sense*. When a blob reproduces, there is mutation chance that one or more of the childs' traits will be increased or decreased in value relative to the parent. As the population grows and competition increases, blobs with less preferable traits die out, and the population evolves towards optimal traits for that environment.

 # Requirements
