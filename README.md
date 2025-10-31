# C++ File System 
> In progress


<table> <tr> <td>

```mermaid
graph TD
    root["(root)"]
    a["a.txt"]
    b["b"]
    c["c.txt"]
    d["d.txt"]
    e["e"]
    bb1["bb1"]
    bb2["bb2"]
    bbb["bbb.txt"]
    ee["ee.txt"]

    root --> a
    root --> b
    root --> c
    root --> d
    root --> e

    b --> bb1
    b --> bb2
    bb1 --> bbb
    e --> ee
```
</td> <td>
  
```mermaid
graph TD
    root["(root)"]
    a["a.txt"]
    b["b"]
    c["c.txt"]
    d["d.txt"]
    e["e"]
    bb1["bb1"]
    bb2["bb2"]
    bbb["bbb.txt"]
    ee["ee.txt"]

    %% Leftmost child links (blue)
    root -->|Leftmost child| a
    b -->|Leftmost child| bb1
    bb1 -->|Leftmost child| bbb
    e -->|Leftmost child| ee

    %% Right sibling links (red)
    a -.->|Right sibling| b
    b -.->|Right sibling| c
    c -.->|Right sibling| d
    d -.->|Right sibling| e
    bb1 -.->|Right sibling| bb2
```
</td> </tr> </table>


## Supports:
- cd
- ls
- pwd
- touch
- mkdir
- rm
- rmdir
- mv  
