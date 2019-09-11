# nat-selection
This is a recreation of the natural selection simulation demonstrated in a [youtube video](https://www.youtube.com/watch?v=0ZGbIKd0XrM), where creatures compete and thier traits evolve depending on environment pressure. Primer has his own [github repository](https://github.com/Helpsypoo/primer), but none of his code was used here. The idea was to copy the simulation "rules" set out in the video and independently recreate it in C++. The simulation runs in its entirety offscreen, then [gnuplot](http://www.gnuplot.info/) is used, via the [gnuplot-iostream api](https://github.com/dstahlke/gnuplot-iostream), to produce plots of how the population number and the creature traits changed. Additionally, a 2D animation of the simulation is displayed, using the [olcPixelGameEngine library](https://github.com/OneLoneCoder/olcPixelGameEngine), as each generated frame is passed to [ffmpeg](https://ffmpeg.org/) to create a local video. 

#### **Example Graphs**

 <img align="left" src="/examples/AverageChange.png" alt="Average change plot for a single simulation" title="Average change plot for a single simulation" width="400"  /> 
 <img align="right" src="/examples/AverageChange_10runs.png" alt="Average change plot for 10 repeat simulation runs" title="Average change plot for 10 repeat simulation runs" width="400" />

<p align="center">
<img align="center" src="/examples/StatDistribution_Sim0.gif" alt="Trait distribution for a given simulation" title="Trait distribution for a given simulation" width="450"><br \>
 </p>
 
 #### **Example Animations**
 <p align="center">
 <img align ="center" src="/examples/blobColours.png" alt="Blob creature art" title="Blob creature art" width="270"><br \>
</p>

<p align="center">
 <img src="/examples/mapSize8_25fps.gif" alt="" title="Simulation with mapSize of 8 (64 grid spaces)" width="409"  />
 <img src="/examples/test.png" alt="" title="" width ="200" />
 <img src="/examples/mapSize25_25fps.gif" alt="" title="Simulation with mapSize of 25 (125 grid spaces)" width="342" /><br \>
</p>

### **Simulation Rules** 
The simulation creates a squared grid map, with "blob" creatures living at the edges, and randomly populates it with food. Each day the blob creatures leave home to hunt for food. If a blob eats one food object and makes it home, it will survive to the next day. If a blob eats two "food" and makes it home then they reproduce asexually. Each blob has three traits to help it hunt food:
 - Size  - A blob can eat another blob so long as the prey is 20% smaller than it.
 - Speed - The number of grid spaces or "steps" it can take at once.
 - Sense - The radius of grid spaces around it that a blob can see. 
 
However, each of these traits has an energy cost. A blob starts each day with an equal amount of energy and each step taken has an energy cost, where *cost = size<sup>3</sup>speed<sup>2</sup>+sense*. When a blob reproduces, there is mutation chance that one or more of the childs' traits will be increased or decreased in value relative to the parent. As the population grows and competition increases, blobs with less preferable traits die out, and the population evolves towards optimal traits for that environment.

 ## External Requirements To Run
  - Install gnuplot *(last tested with version 5.2.7)*
  - Install ffmpeg *(last tested with windows build 4.2)*
  - The boost C++ libraries *(tested with version 1.70.0)*

  #### gnuplot installation
The latest version of gnuplot can be found [here](http://www.gnuplot.info/download.html), last tested version [here](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.2.7/). Gnuplot just needs to be installed on the system such that "gnuplot" is a recognised command in cmd/terminal. In windows (my system) I used the "*gp527-win64-mingw.exe*" installer, ensuring to check the "*Add application directory to your PATH environment variable*" option. Otherwise gnuplot cannot be called from cmd.

  #### ffmpeg installation
The latest version of ffmpeg can be found [here](https://ffmpeg.org/download.html).

  #### boost libraries
The gnuplot-iosteam api is dependent on the boost libraries. The boost header (.h) files must be visible to the compiler, and the library (.lib and .dll) files visible to the linker. If they are not made available to the IDE the code will not compile. The following instructions are for Visual Studio (the IDE I used).
- Download the latest [boost version](https://www.boost.org/users/download) (last tested version [here](https://sourceforge.net/projects/boost/files/boost/1.70.0/)) and extract.
- To add the header files to the include directory, in the toolbar, open *Project -> Properties -> VC++ Directories -> Include Directories*.
- Click the dropdown arrow then *<Edit...>* and add the path to the overall boost folder, in my case *C:\boost_1_70_0* 
- To add the library files, in the toolbar open *Project -> Properties -> Linker -> General -> Additional Library Directories*
-  Click the dropdown arrow then *<Edit...>* and add the path to the *lib* and *lib64-msvc-14.1* folders, e.g. *C:\boost_1_70_0\lib* and *C:\boost_1_70_0\lib64-msvc-14.1* 

**Note:** I kept encountering linker errors with the \lib files when trying x86 builds within Visual Studio. Not sure wether this is a boost library problem, a problem with gnuplot-iostream on windows, or my own mistake. But rather than fix it, I just removed the lib path (kept the lib64 path) and always compiled x64 builds.

## Art
The pixel art for the blob creatures and food were both adapted from free resources on itch.io, found [here](https://stealthix.itch.io/animated-slimes) and [here](https://vectorpixelstar.itch.io/8x8px-icons) respectively.
