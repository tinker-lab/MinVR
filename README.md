#MinVR


##Introduction

MinVR is an Open Source Project developed and maintained collaboratively by the University of Minnesota's Interactive Visualization Lab and the Brown University Visualization Research Lab.

The goal of MinVR is to faciliate a variety of data visualization and virtual reality research projects by providing a robust, cross-platform VR toolkit for use with many different VR displays (e.g., CAVE's, PowerWalls, multi-touch stereoscopic tables, 3DTV's, head-mounted displays) and input devices (e.g., 6 degree-of-freedom trackers, multi-touch input devices, haptic devices, home-built devices).  

MinVR has (so far) been tailored to support devices and displays at the University of Minnesota IV/LAB and Brown University's VRL.  Eventually, our labs would be happy if other groups also find this low-level library useful.  A great vision for this library would be for it to one day become a standard (at least within the academic research community) similar to tools, such as VRPN, TUIO, and VTK.

At the IEEE VR 2014 conference, several of us connected to the MinVR project and its predecessors particiated in a panel on VR toolkits.  The title of the panel was something like, "VR Toolkits: Why do we keep reinventing the wheel?"  This is a fair question to ask.  After all, wouldn't we all be better off if we pooled our resources behind some existing VR toolkit?  One of the best answers to the question "why reinvent the wheel?" that emerged in this panel was something along this lines of this:  We reinvent the wheel because we (grad students in particular) learn a lot through this process and frankly is nearly as easy to write your own VR toolkit from scratch as it is to learn how to use somebody else's!  Perhaps one day we will reach the point where the stars align: somebody gets the design of a VR toolkit "correct enough" that many people want to use it, the toolkit is publicized enough that many people hear about it, and the interest in VR is high enough that the toolkit takes off and becomes a standard with great documentation, suppport, etc.  Until that time, perhaps it's not a bad thing that many research labs have developed their own toolkits -- through this process we are all continuing to refine our ideas about what it is that makes a great VR toolkit.

MinVR is the result of many lessons learned about VR software, but there are two lessons that are most important and really guide the philosophy of MinVR.  

Lesson #1: Keep it small.  The "Min" in MinVR comes from the lesson that a good VR toolkit is one that is small.  All VR toolkits start small, but since most are created by a group of grad students as they study, what seems to happen is that the toolkit grows with every new research project the students do.  New graphics routines, networking code, audio toolkits, CAVE simulators, etc. get added as required by various research projects.  This is fine, and probably the quickest way to get the research projects done, but the result is a big toolkit that is difficult for others to reuse.  

Lesson #2: Keep it flexible.  We don't know of any other VR toolkits that are designed to work with more than one graphics engine.  To be fair, MinVR does assume that the graphics are based on OpenGL, but that still provides flexibility to use MinVR with graphics toolkits, such as G3D, OpenHaptics, VTK, and OpenSceneGraph, just to name a few.  In the past, we've found this to be a major limitation of other VR toolkits.  What if you want to use VTK's volume rendering funcationality in your reserach project, but your VR toolkit requires you to use OpenSceneGraph.  Theoretically, VTK and OSG should be compatible because they both use OpenGL, but in practice, each toolkit will want to be in charge of things like opening a stereoscopic graphics window when the program starts up, managing the OpenGL state, and calling swapbuffers().  It is very difficult to mix toolkits in this way, but we find that most of the graphics toolkits we want to use don't do a good enough job of supporting VR out of the box.  Instead of picking one toolkit, MinVR is flexible - plug in whatever toolkit you want.  Similarly, MinVR tries to avoid forcing developers to pick a particular VR display or input device.  Some other VR toolkits work only for CAVEs or only for HMDs.  If you are a researcher studying many different types of displays, then this is a bummer.  So, MinVR provides a lot of flexibility over the type of virtual camera that is used for rendering.  Finally, MinVR tries to provide the same type of flexibility for input devices.  Older VR toolkits sometimes forced developers into a mindset of having a VR wand as the "standard" input device.  If you are a researcher studying many different types of user input, then this is limiting.  MinVR uses a very generic model for input events with the idea that if you write your code to respond to an event of a particular name, then we should be able to swap in a number of different input devices that can generate that event, and these should all work quite easily.  If you write your code with this in mind, you can often make a desktop version of your software that works just like the VR version -- just "fake" the VR input events with keyboard, mouse, or other desktop inputs that you rename to act as though they are from VR devices.  In this way, doesn't require a separate "simulator mode" used during development to simulate how a program would run in VR.  Instead, just setup your config files correctly to "fake" VR at your desktop.  The example projects distributed with the library (will soon) demonstrate this.


##History and Related Projects

MinVR itself was designed based on the lessons described above and created at the University of Minnesota in 2013, but it can also be viewed as essentially a major refactoring of a previous project, VRG3D (also released under a BSD-3 license), that was developed at Brown University starting around 2004, and was co-developed and maintained by Brown and Minnesota from 2008-2014.  In the MinVR Core library, you will still see some references to VRG3D.  VRG3D is a great toolkit that was used very successfully for many VR and visualization research projects, but it has a very strong dependency on the open source graphics library, G3D.  G3D is great for working with the latest graphics card hardware and rendering amazing looking graphics, but it does not cooperate well with some of the other graphics libraries mentioned above (VTK, OSG, OpenHaptics, etc.), which made it very difficult to do VR research projects with any graphics libary other than G3D.  These other libraries are particularly valuable for research on data visualization, so this provided strong motivation for creating MinVR.  


##MinVR Features and Design 

Core Library Features:
- window configuration
- stereo output for various display devices
- input device configuration
- input events for VR, multi-touch, and other non-keyboard, mouse, joystick events based on VRPN
- setting up head tracking and appropriate projection matrices   
- multi-threaded rendering with support for multiple graphics windows (i.e., multiple graphics contexts), one per thread


###Dependencies and Design Philosophy

MinVR's primary design philosophy is to minimize potential dependency conflicts with other libraries you may be using. 

The core library depends on [Boost](http://boost.org) and the header-only OpenGL Math Library (http://glm.g-truc.net).  Boost is rather large, but it is so widely used that it is not a risky dependency.  GLM is used because, being header-only, it is quite lightweight and doesn't introduce a need to build and link against another library just to have reasonable support for graphics math (vectors, matrices, etc).

There are also a series of optional dependencies that you can select depending upon exactly what graphics toolkit you would like to use and what input device(s) you wish to support on your platform.  

Different graphics toolkits are supported via "App Kits".  There are app kits for libraries ranging from the very minimal (e.g., glut) to the very specific (e.g., OpenHaptics).

Input devices are supported via optional features that can be compiled into the core library.  Examples inclue the use of [VRPN](http://www.cs.unc.edu/Research/vrpn/) for accessing analog, button, and tracking input data via a VRPN server running on your local network and [TUIO](http://http://www.tuio.org/) for accessing mulit-touch input data via a similar client-server approach.


##Documentation

For API documentation and tutorials, see the [MinVR Documentation](http://minvr.github.io/MinVR/docs/latest/index.html).


