Compilation:

No changes from the standard compilation procedure are necessary.
cd to the A4 directory, run premake4 gmake, then make.
Then, cd to the Assets/ directory and run ../A4 <input scene> to run.

Tested on gl17 in MC3007

---------------------------------------------------------------------------------------------

Manual:

I did not have time to implement:
-hierarchical transformations
-triangle mesh intersection rendering (I almost had it - please see Mesh::intersect in Mesh.cpp)
-bounding volumes
-any extra features

I have included the following renderings:
screenshot.png
non-hier.png

I am unable include the following renderings:
macho-cows.png
simple-cows.png
nonhier-bb.png