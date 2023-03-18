int BSTreeHeight(BSTree t) {
    if (t == NULL || (t -> left == NULL && t -> right == NULL)) {
        return 0;
    }
    int rh = BSTreeHeight(t -> right);
    int lh = BSTreeHeight(t -> left);
    else if (lh > rh) {
        return 1 + lh;
    }
    return 1 + rh;
}