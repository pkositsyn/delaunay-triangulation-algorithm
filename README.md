# Delaunay-Triangulation-Algorithm

This is my iterative algorithm for Delaunay triangulation.

The idea is to sort all the points by their x-coordinate and iteratively add in the triangulation. Firstly, we find all "visible" points in current triangulation from the new point. Then we make edges between them and new point. Finally, for each new triangle the Delaunay condition is checked and recursively applied to the triangles appeared in case of changes.

I assume this algorithm to work for O( N log N ).
Points are sorted in O( N log N ).
Finding the "visible" points is O( N ) due to the fact that the last added point before the current is always "visible". So we can move to the left and to the right from it and will find only 2 points which aren't "visible". The total number of the points we check is the number of triangles we produce + 2 * N (we don't change the number of triangles in checking Delaunay condition). As long as number of triangles is O( N ) the proof is complete.
There is a tough situation with total number of recursive calls for checking but I saw in some works that it is also O( N ), though I can't confirm this fact.

Here are approximate tests time result in seconds:
![screenshot from 2018-11-30 19-02-01](https://user-images.githubusercontent.com/37667546/49300149-89fa6c80-f4d2-11e8-897d-950c35f907b7.png)

Attaching some nice pictures of triangulations made by this program:
(blue are the edges which were removed during algorithm)

### Uniform variation, 1000 points
![screenshot from 2018-11-30 18-15-20](https://user-images.githubusercontent.com/37667546/49297477-ec9c3a00-f4cb-11e8-86c2-589c0dbee38d.png)
### Normal variation, 1000 points
![screenshot from 2018-11-30 17-46-50](https://user-images.githubusercontent.com/37667546/49296028-41d64c80-f4c8-11e8-98f6-2e076ea267fb.png)
The only bad thing I see in this algorithm that it produces a lot of narrow long triangles, which can be bad for precision.
(The same points set as on the previous picture)
![screenshot from 2018-11-30 18-02-10](https://user-images.githubusercontent.com/37667546/49297109-0426f300-f4cb-11e8-8c39-499d95afe935.png)
### Pretty pattern
![screenshot from 2018-11-30 17-43-27](https://user-images.githubusercontent.com/37667546/49296030-41d64c80-f4c8-11e8-86af-2143deac856f.png)
### A circle with a point in the center
![screenshot from 2018-11-30 17-39-33](https://user-images.githubusercontent.com/37667546/49305179-989b5080-f4df-11e8-8faf-2a366ea22ad4.png)


### Ellipse
![screenshot from 2018-11-30 18-00-33](https://user-images.githubusercontent.com/37667546/49297152-24ef4880-f4cb-11e8-8dec-539e5b975f3a.png)
