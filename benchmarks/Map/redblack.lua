--------------------------------------------------------------------------------
-- Copyright (C) 2014, Greg Johnson.
-- Released under the terms of the GNU GPL v2.0.
--------------------------------------------------------------------------------
--[[
This is a lua implementation of red black trees.

Here are the provided operations:
 - create a new tree
 - insert a data element
 - delete a data element
 - find a data element
 - iterate over the data elements
 
A red black tree is a binary search tree in which each node is marked
as a red node or a black node.  The tree satisfies the following two
properties, which together guarantee that insert, delete, and find
operations are all O(logN):

    Red property:    Red nodes do not have red parents.

    Black property:  Siblings have the same maximum black height.

(The maximum black height of a node is the maximum number of black nodes
on any simple path starting at the node and descending into its subtree.)

Data values are any lua values that support comparison via "<", including
tables with metatables containing the "__lt" method.

Usage:
    require 'redblack'
    local tree = redblack.newTree()

    redblack.insert(tree, 10)
    redblack.insert(tree, 20)

    for value in redblack.iterate(tree) do
        print(value)
    end

    print(redblack.find(tree, 10))        -- expect 10

    redblack.delete(tree, 10)
    redblack.delete(tree, 20)

    print(redblack.find(tree, 10) ~= nil) -- expect false
--]]

local delete, deleteNode, farNephew, findNode, first, grandparent, insert
local insertIntoSortedPosition, insideChild, isBlackNode, isInsideChild
local isLeftChild, isRedNode, isRightChild, isRootNode, iterate
local leftChild, ensureFarNephewIsRed, ensureOutsideChild, makeRootNode
local ensureSiblingIsBlack, nearNephew, newNode, newTree, outsideChild
local parent, restoreBlackProperty, restoreRedProperty, rightChild
local rotateUp, rotateUpBlackNode, setChild, sibling, successor, swapColors
local swapWithSuccessor, uncle, violatesBlackProperty, violatesRedProperty
local getChildOrNil, getOnlyChild, find, ensureLeafOrParentOfOneLeaf

------------------------------- public functions -------------------------------
function newTree()
    return { childList = {} }
end

-- insert a node into the tree; data values must be comparable using "<".
--
function insert(tree, data)
    local insertedNode = insertIntoSortedPosition(tree, tree.root, data)

    if violatesRedProperty(insertedNode) then
        restoreRedProperty(tree, insertedNode)
    end
end

-- delete a node from the tree; data values must be comparable using "<".
--
function delete(tree, data)
    local deleteMe = findNode(tree.root, data)

    if deleteMe == nil then return end

    deleteMe = ensureLeafOrParentOfOneLeaf(deleteMe)

    if not isRootNode(deleteMe) and not isRedNode(deleteMe) then
        deleteMe.color = 'white'  -- to satisfy pre-condition of restoreBlackProperty()
        restoreBlackProperty(tree, deleteMe)
    end

    deleteNode(tree, deleteMe)
end

function find(tree, data)
    local node = findNode(tree.root, data)
    return node and node.data
end

function iterate(xTree)
    local f = function(s, var)
                 local result = s.returnMe
                 s.returnMe = successor(s.returnMe)
                 return result and result.data
              end

    return f, { returnMe = first(xTree.root) }, nil
end
----------------------------- end public functions -----------------------------

-- Node "fixMe" is red and has a red parent, violating the red property.
-- Other than that, all nodes in the tree satisfy the red and black properties.
--
-- Upon return, the red property is restored for all nodes in the tree.  
--
function restoreRedProperty(tree, fixMe)
    -- base case 1:
        if isRootNode(parent(fixMe)) then
            parent(fixMe).color = 'black'

    -- base case 2:
        elseif not isRedNode(uncle(fixMe)) then
            -- rotateUp changes color of outside child's parent.
            -- So, if fixMe is outside child, then rotateUp(parent of fixMe) will fix red violation.
            --
            -- (rotateUp requires a red node with black sibling as input)

            fixMe = ensureOutsideChild(tree, fixMe)

            rotateUp(tree, parent(fixMe))

    -- inductive case:
        else
            parent(fixMe).color      = 'black'
            uncle(fixMe).color       = 'black'
            grandparent(fixMe).color = 'red'

            if violatesRedProperty(grandparent(fixMe)) then
                restoreRedProperty(tree, grandparent(fixMe))
            end
        end
end

-- Node fixMe has max black height one less than its sibling, violating the black property.
-- Other than that, all nodes in the tree satisfy the red and black properties.
-- (This algorithm also requires that fixMe not be a red node.)
--
-- Upon return, the black property is restored for all nodes in the tree.  
--
-- (Also, upon return the parent of fixMe (if it exists) is black;
-- this fact is used to slightly simplify the delete algorithm.)
--
function restoreBlackProperty(tree, fixMe)
    ensureSiblingIsBlack(tree, fixMe)

    -- base case 1:
        if isRedNode(nearNephew(fixMe)) or isRedNode(farNephew(fixMe)) then
            -- In this case we can treat parent(fixMe) as root of a tree; no recursion will be necessary.
            -- rotateUpBlackNode(sibling) will swap max black heights of root's two subtrees.

            -- rotate operation will make far nephew a child of root and uncle of fixMe.
            -- If we ensure that far nephew is red, we will be able to increase its max
            -- black height by simply changing its color to black.

            ensureFarNephewIsRed(fixMe)

            rotateUpBlackNode(tree, sibling(fixMe))

            uncle(fixMe).color = 'black'
        else
            -- Fix black property violation by reducing sibling's max black height.  (This also
            -- reduces max black height of parent, potentially giving parent a black violation.)

            sibling(fixMe).color = 'red'

    -- base case 2:
            if isRedNode(parent(fixMe)) then
                parent(fixMe).color = 'black'

    -- inductive case:
            elseif not isRootNode(parent(fixMe)) then
                restoreBlackProperty(tree, parent(fixMe))
            end
        end
end

----------------------------- rotateUp operations ------------------------------
-- pre-condition:  redNode must be red and must not have a red sibling.
-- This operation preserves the red and black properties of the tree and the
-- in-order traversal of the tree.
--
--     orig_parent (b)              node (b)
--          |                        |         
--     +----+----+     ===>     +----+----+    
--     |         |              |         |    
--    node (r)   t3             t1   orig_parent (r)
--     |                                  |    
--  +--+--+                            +--+--+ 
--  |     |                            |     | 
--  t1    t2                           t2    t3
--
function rotateUp(tree, redNode)
    assert(parent(redNode))

    swapColors(redNode, parent(redNode))

    local leftChild = isLeftChild(redNode)
    local p  = parent(redNode)
    local gp = grandparent(redNode)

    setChild(tree, p,         insideChild(redNode), leftChild)
    setChild(tree, gp,        redNode,              isLeftChild(p))
    setChild(tree, redNode,   p,                    not leftChild)
end

-- pre-condition:  blackNode must be black and must not have a red outside child.
-- This function preserves the red property and the in-order traversal of the tree,
-- but it violates the black property:
--
-- if we consider parent(blackNode) to be the root of a subtree,
-- rotateUpBlackNode(node) swaps max black heights of root's two subtrees.
--
-- (This function is identical to rotateUp(), except for the pre-conditions)
--
function rotateUpBlackNode(tree, blackNode)
    rotateUp(tree, blackNode)
end
--------------------------- end rotateUp operations ----------------------------

------------------------------- iterator support -------------------------------
function first(xNode)
    while leftChild(xNode) do
        xNode = leftChild(xNode)
    end
    return xNode
end

function successor(xNode)
    local result = first(rightChild(xNode))
    if result == nil then
        while isRightChild(xNode) do
            xNode = parent(xNode)
        end
        result = parent(xNode)
    end
    return result
end
----------------------------- end iterator support -----------------------------

------------------------------ familial relations ------------------------------
function parent(node)
    return node and node.parent
end

function sibling(node)
    if     isLeftChild(node)  then return rightChild(parent(node))
    elseif isRightChild(node) then return leftChild(parent(node))
    else                           return nil
    end
end

function insideChild(node)
    if     isLeftChild(node)  then return node.childList[2]
    elseif isRightChild(node) then return node.childList[1]
    else                           return nil
    end
end

function outsideChild(node)
    if     isLeftChild(node)  then return node.childList[1]
    elseif isRightChild(node) then return node.childList[2]
    else                           return nil
    end
end

function grandparent(node)
    return parent(parent(node))
end

function uncle(node)
    return sibling(parent(node))
end

function nearNephew(node)
    return insideChild(sibling(node))
end

function farNephew(node)
    return outsideChild(sibling(node))
end

function getOnlyChild(node)
     return (node.childList[1] or node.childList[2])
end

function isInsideChild(node)
    return    isLeftChild(parent(node))  and isRightChild(node)
           or isRightChild(parent(node)) and isLeftChild(node)
end

function ensureOutsideChild(tree, node)
    if isInsideChild(node) then
        rotateUp(tree, node)
        node = outsideChild(node)
    end
    return node
end

function leftChild(node)
    return node and node.childList[1]
end

function rightChild(node)
    return node and node.childList[2]
end

function isLeftChild(child)
    return child and child == leftChild(parent(child))
end

function isRightChild(child)
    return child and child == rightChild(parent(child))
end

function setChild(tree, parentNode, childNode, makeLeftChild)
    if parentNode == nil then
        tree.root = childNode

    elseif makeLeftChild then
        parentNode.childList[1] = childNode

    else
        parentNode.childList[2] = childNode
    end

    if childNode ~= nil then
        childNode.parent = parentNode
    end

    return childNode
end

function isRootNode(node)
    return node and node.parent == nil
end

function makeRootNode(tree, node)
    tree.root = node
    return node
end
---------------------------- end familial relations ----------------------------

------------------------------ support functions -------------------------------
function violatesRedProperty(node)
    return isRedNode(node) and isRedNode(parent(node))
end

function findNode(subtreeRoot, data)
    if subtreeRoot == nil then
        return nil

    elseif data < subtreeRoot.data then
        return findNode(subtreeRoot.childList[1], data)

    elseif subtreeRoot.data < data then
        return findNode(subtreeRoot.childList[2], data)

    else
        return subtreeRoot
    end
end

function insertIntoSortedPosition(tree, subtreeRoot, xData)
    if subtreeRoot == nil then 
        return makeRootNode(tree, newNode(xData))

    else
        local childIndex = (xData < subtreeRoot.data and 1 or 2)

        if subtreeRoot.childList[childIndex] == nil then 
            return setChild(tree,
                            subtreeRoot,
                            newNode(xData),
                            childIndex == 1)
        else
            return insertIntoSortedPosition(tree,
                                            subtreeRoot.childList[childIndex],
                                            xData)
        end
    end
end

function newNode(xData)
    local node = { data = xData, color = 'red', childList = {} }
    return node
end

function isBlackNode(node)
    return node and node.color == 'black'
end

function isRedNode(node)
    return node and node.color == 'red'
end

function ensureLeafOrParentOfOneLeaf(deleteMe)
    if deleteMe.childList[1] ~= nil and deleteMe.childList[2] ~= nil then
        deleteMe = swapWithSuccessor(deleteMe)
    end
    return deleteMe
end

function swapColors(node1, node2)
      node1.color, node2.color = node2.color, node1.color
end

function ensureSiblingIsBlack(tree, node)
    if isRedNode(sibling(node)) then
        rotateUp(tree, sibling(node))
    end
end

function ensureFarNephewIsRed(tree, node)
    if isBlackNode(farNephew(node)) then
        rotateUp(tree, nearNephew(node))
    end
end

function swapWithSuccessor(deleteMe)
    local succ = successor(deleteMe)
    deleteMe.data, succ.data = succ.data, deleteMe.data
    return succ
end

function getChildOrNil(node)
     return (node and (node.childList[1] or node.childList[2]))
end

function deleteNode(tree, deleteMe)
    assert(deleteMe ~= nil
           and (deleteMe.childList[1] == nil or deleteMe.childList[2] == nil))

    setChild(tree,
             parent(deleteMe),
             getChildOrNil(deleteMe),
             isLeftChild(deleteMe))
end
---------------------------- end support functions -----------------------------

------------------------- support for testing redblack -------------------------
local function printNode(node)
    io.write(tostring(node))
    io.write('<c1:  ', tostring(node.childList[1]), '>')
    io.write(' <c2:  ', tostring(node.childList[2]), '>')
    io.write(' <color:  ', tostring(node.color), '>')
    io.write(' <data:  ', tostring(node.data), '>')
    --for k,v in pairs(node) do
    --    io.write(' <' .. tostring(k) .. ': ' .. tostring(v) .. '>')
    --end
    print()
end

local function printTree(tree, depth)
    if tree == nil then return end
    if tree.root ~= nil then tree = tree.root end
    if depth == nil then depth = 0 end

    if depth == 0 then print() end

    printTree(tree.childList[1], depth+1)

    for i = 1, depth do io.write("    ") end
    printNode(tree)

    printTree(tree.childList[2], depth+1)

    if depth == 0 then print() end
end

testredblack = {
    insertIntoSortedPosition = insertIntoSortedPosition,
    printNode = printNode,
    printTree = printTree,
}
----------------------- end support for testing redblack -----------------------

redblack = {
    newTree = newTree,
    insert  = insert,
    delete  = delete,
    iterate = iterate,
    find    = find,
}

