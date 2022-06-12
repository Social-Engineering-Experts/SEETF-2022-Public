# Stupid Students - Walkthrough

Viewing the PDF in a hex editor, we can see that /Root references to 26 0 obj. This object is the Root Node of the document.

Going to 26 0 obj, we see that it references to 2 0 obj. Under 2 0 obj, we only see that there are 3 objects referenced under `/Kids`:

- 1 0 obj
- 12 0 obj
- 22 0 obj

`Kids` is a required entry in an intermediate node (i.e. a node that is not a page). Another required entry that is not present here is `/Count`, which is the number of page nodes (leaves) that are eventual children of this node. Note that all three objects above are page nodes, so the `/Count` is 3. We add `/Count 3` to 26 0 obj.

Lets dive into 1 0 obj, 12 0 obj and 22 0 obj respectively.

Each of these pages requires certain entries in their respective page dictionaries. The `/Parent` entry references the Parent object and `/Contents` entry reference sthe data stream for this object - both are missing from these objects.

We need to add `/Parent 2 0 R` to each of these objects, since they are children nodes of 2 0 obj.

We also notice that the 3 0, 13 0 and 23 0 data streams are not referenced in any other objects.

This means they are probably the actual data in the PDF. We need to add `/Contents 3 0 R`, `Contents 13 0 R` and `Contents 23 0 R` respectively to each of the objects.

## References

- https://www.oreilly.com/library/view/pdf-explained/9781449321581/ch04.html
