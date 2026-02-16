#include "../elhaylib.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

#define TEST_NAMES_LENGTH 25
#define NO_YAPPING true
#define PRINT_TREES true

void print_test_res(bool result) {
    if(result) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
}

void printTree(tree_node* ptr_node, int depth, bool isLast, bool* flag);

tree_op_res result = {0};
tree_head my_tree = {0};
int my_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15};

tree_head my_other_tree = {0};

/*
/   Build following tree:
/                  0
/                /   \
/               /     \
/              1       3
/             / \    /   \
/            4   5  6     8
*/
bool build_my_tree(bool no_yapping) {
    tree_init(&my_tree);
    tree_node_root(&result, &my_tree, NODE_INT, sizeof(int), &my_data[0]);
    tree_node* root_node = result.node_ptr;

    if(!no_yapping) {
        printf("1. Testing tree build/add:\n");
    }

    if(!no_yapping) {
        printf("Node 0: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)root_node, (void*)root_node->parent,
               root_node->children.dynarr_size, (int)*root_node->data);
    }

    tree_node_add(&result, &my_tree, root_node, NODE_INT, sizeof(int),
                  &my_data[1]);
    tree_node* one_node = result.node_ptr;

    if(!no_yapping) {
        printf("Node 0: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)root_node, (void*)root_node->parent,
               root_node->children.dynarr_size, (int)*root_node->data);
    }

    tree_node_add(&result, &my_tree, root_node, NODE_INT, sizeof(int),
                  &my_data[3]);
    tree_node* three_node = result.node_ptr;

    if(!no_yapping) {
        printf("Node 0: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)root_node, (void*)root_node->parent,
               root_node->children.dynarr_size, (int)*root_node->data);
    }

    tree_node_add(&result, &my_tree, one_node, NODE_INT, sizeof(int),
                  &my_data[4]);

    if(!no_yapping) {
        printf("Node 1: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)one_node, (void*)one_node->parent,
               one_node->children.dynarr_size, (int)*one_node->data);
    }

    tree_node_add(&result, &my_tree, one_node, NODE_INT, sizeof(int),
                  &my_data[5]);

    if(!no_yapping) {
        printf("Node 1: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)one_node, (void*)one_node->parent,
               one_node->children.dynarr_size, (int)*one_node->data);

        tree_node* four_node = tree_get_ith_node_ptr(one_node, 0);
        printf("Node 4: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)four_node, (void*)four_node->parent,
               four_node->children.dynarr_size, (int)*four_node->data);

        tree_node* five_node = tree_get_ith_node_ptr(one_node, 1);
        printf("Node 5: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)five_node, (void*)five_node->parent,
               five_node->children.dynarr_size, (int)*five_node->data);
    }

    tree_node_add(&result, &my_tree, three_node, NODE_INT, sizeof(int),
                  &my_data[6]);

    if(!no_yapping) {
        printf("Node 3: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)three_node, (void*)three_node->parent,
               three_node->children.dynarr_size, (int)*three_node->data);
    }

    tree_node_add(&result, &my_tree, three_node, NODE_INT, sizeof(int),
                  &my_data[8]);

    if(!no_yapping) {
        printf("Node 3: self: %p - parent: %p - n-children: %zu - data: %d\n",
               (void*)three_node, (void*)three_node->parent,
               three_node->children.dynarr_size, (int)*three_node->data);
    }

    // naive return - non exhaustive
    return root_node == one_node->parent;
}

/* Transform tree by inserting nodes:
/                  0                        0
/                /   \                    / | \
/               /     \                  /  |  \
/              1       3       --->     1   2   3
/             / \    /   \             / \    / | \
/            4   5  6     8           4   5  6  7  8
/                                               |
/                                               9
*/
bool test_insert_nodes(bool no_yapping) {
    tree_node* root_node = my_tree.tree_root;
    tree_node_add_at_index(&result, &my_tree, root_node, 1, NODE_INT,
                           sizeof(int), &my_data[2]);
    bool sub_test_1 = result.code == OK;

    tree_node* three_node = tree_get_ith_node_ptr(root_node, 2);
    tree_node_add_at_index(&result, &my_tree, three_node, 1, NODE_INT,
                           sizeof(int), &my_data[7]);
    bool sub_test_2 = result.code == OK;

    tree_node* seven_node = result.node_ptr;
    tree_node_add(&result, &my_tree, seven_node, NODE_INT, sizeof(int),
                  &my_data[9]);
    bool sub_test_3 = result.code == OK;

    if(!no_yapping) {
        printf("2. Testing inserts\n");
        printf("%*s", TEST_NAMES_LENGTH, "1st Insert OK: ");
        print_test_res(sub_test_1);
        printf("%*s", TEST_NAMES_LENGTH, "2nd Insert OK: ");
        print_test_res(sub_test_2);
        printf("%*s", TEST_NAMES_LENGTH, "3rd Add OK: ");
        print_test_res(sub_test_3);
    }

    tree_node* nine_node = result.node_ptr;

    // naive return - non exhaustive
    return (int)*nine_node->data == 9;
}

bool test_count_nodes(bool no_yapping) {
    bool test_result = true;
    bool sub_tests[] = {true, true, true, true};
    const int n_sub = 4;

    if(!no_yapping) {
        printf("3a. Testing subtree counts:\n");
        printf("      Expecting: 3 - 1 - 5\n");
    }

    tree_node* root = my_tree.tree_root;
    tree_node* left = tree_get_ith_node_ptr(root, 0);
    tree_node* mid = tree_get_ith_node_ptr(root, 1);
    tree_node* right = tree_get_ith_node_ptr(root, 2);

    size_t cnt_left = tree_count_nodes(left);
    size_t cnt_mid = tree_count_nodes(mid);
    size_t cnt_right = tree_count_nodes(right);

    sub_tests[0] &= cnt_left == 3;
    sub_tests[1] &= cnt_mid == 1;
    sub_tests[2] &= cnt_right == 5;

    if(!no_yapping) {
        printf("             is: %zu - %zu - %zu\n", cnt_left, cnt_mid,
               cnt_right);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0] && sub_tests[1] && sub_tests[2]);
    }

    if(!no_yapping) {
        printf("3b. Testing full tree counts:\n");
        printf("      Expecting: 10\n");
    }

    size_t head_cnt = my_tree.tree_size;
    size_t cnt_counted = tree_count_nodes(root);

    sub_tests[3] &= head_cnt == cnt_counted;
    sub_tests[3] &= 10 == cnt_counted;

    if(!no_yapping) {
        printf("           Size: %zu - Count: %zu\n", head_cnt, cnt_counted);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[3]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_detach_subtree(bool no_yapping) {
    bool test_result = true;

    bool sub_tests[] = {true, true, true};
    const int n_sub = 3;

    tree_init(&my_other_tree);
    tree_node* detached_subtree = tree_get_ith_node_ptr(my_tree.tree_root, 0);

    tree_detach_subtree(&result, &my_tree, detached_subtree);
    tree_graft_root(&result, &my_other_tree, result.node_ptr);

    sub_tests[0] = my_tree.tree_size == 7;
    sub_tests[1] = my_other_tree.tree_size == 3;

    if(!no_yapping) {
        printf("4a. Testing subtree detach:\n");
        printf("Tree counts:\n");
        printf("      Expecting: 7 - 3\n");
        printf("             Is: %zu - %zu\n", my_tree.tree_size,
               my_other_tree.tree_size);
        printf("%*s", TEST_NAMES_LENGTH, "Result 1: ");
        print_test_res(sub_tests[0]);
        printf("%*s", TEST_NAMES_LENGTH, "Result 2: ");
        print_test_res(sub_tests[1]);
    }

    int16_t test = 10;
    tree_node* floating_node =
        tree_prepare_node(NODE_INT16, sizeof(int16_t), &test);
    tree_detach_subtree(&result, &my_tree, floating_node);
    sub_tests[2] = result.code == SUBTREE_UNATTACHED;

    if(!no_yapping) {
        printf("4b. Testing subtree detach protection:\n");
        printf("Try to detach a subtree that has no parent.\n");
        printf("   Expecting %d Code.\n", SUBTREE_UNATTACHED);
        printf("          Is %d Code.\n", result.code);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_graft_subtree(bool no_yapping) {
    bool test_result = true;

    bool sub_tests[] = {true, true, true};
    const int n_sub = 3;

    tree_detach_root(&result, &my_other_tree);
    tree_graft_subtree(&result, &my_tree, my_tree.tree_root, result.node_ptr,
                       0);
    sub_tests[0] = my_tree.tree_size == 10;
    sub_tests[1] = my_other_tree.tree_size == 0;

    if(!no_yapping) {
        printf("5a. Testing subtree graft:\n");
        printf("Tree counts:\n");
        printf("      Expecting: 10 - 0\n");
        printf("             Is: %zu - %zu\n", my_tree.tree_size,
               my_other_tree.tree_size);
        printf("%*s", TEST_NAMES_LENGTH, "Result 1: ");
        print_test_res(sub_tests[0]);
        printf("%*s", TEST_NAMES_LENGTH, "Result 2: ");
        print_test_res(sub_tests[1]);
    }

    tree_graft_subtree(&result, &my_tree, my_tree.tree_root, result.node_ptr,
                       0);
    sub_tests[2] = result.code == SUBTREE_ATTACHED;

    if(!no_yapping) {
        printf("5b. Testing subtree graft protection:\n");
        printf("Try to graft a subtree that is not free floating.\n");
        printf("   Expecting %d Code.\n", SUBTREE_ATTACHED);
        printf("          Is %d Code.\n", result.code);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    bool flags[10] = {true};
    memset(flags, true, 10);

    bool no_yapping = NO_YAPPING;
    bool print_trees = PRINT_TREES;

    bool test_result1 = build_my_tree(no_yapping);
    if(!no_yapping || print_trees) {
        printf("\nState ONE: Initial Tree:\n");
        printTree(my_tree.tree_root, 0, false, flags);
        memset(flags, true, 10);
    }

    bool test_result2 = test_insert_nodes(no_yapping);
    if(!no_yapping || print_trees) {
        printf("\nState TWO: Insert one nodes and two leaf:\n");
        printTree(my_tree.tree_root, 0, false, flags);
        memset(flags, true, 10);
    }

    bool test_result3 = test_count_nodes(no_yapping);

    bool test_result4 = test_detach_subtree(no_yapping);
    if(!no_yapping || print_trees) {
        printf("\nState TWO: detach subtree:\n");
        printf("Tree One:\n");
        printTree(my_tree.tree_root, 0, false, flags);
        memset(flags, true, 10);
        printf("Tree Two:\n");
        printTree(my_other_tree.tree_root, 0, false, flags);
        memset(flags, true, 10);
    }

    bool test_result5 = test_graft_subtree(no_yapping);
    if(!no_yapping || print_trees) {
        printf("\nState THREE: Reconstituted tree:\n");
        printf("Tree One:\n");
        printTree(my_tree.tree_root, 0, false, flags);
        memset(flags, true, 10);
        printf("Tree Two:\n");
        printTree(my_other_tree.tree_root, 0, false, flags);
        memset(flags, true, 10);
    }

    printf("Tree Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "Tree build/add Test: ");
    print_test_res(test_result1);
    printf("%*s", TEST_NAMES_LENGTH, "Node Insertion Test: ");
    print_test_res(test_result2);
    printf("%*s", TEST_NAMES_LENGTH, "Tree count Test: ");
    print_test_res(test_result3);
    printf("%*s", TEST_NAMES_LENGTH, "Subtree detach Test: ");
    print_test_res(test_result4);
    printf("%*s", TEST_NAMES_LENGTH, "Subtree graft Test: ");
    print_test_res(test_result5);
}

// adapted from: https://www.geeksforgeeks.org/dsa/print-n-ary-tree-graphically/
void printTree(tree_node* ptr_node, int depth, bool isLast, bool* flag) {
    if(ptr_node == NULL) {
        printf("[]\n");
        return;
    }

    // Loop to print the depths of the
    // current node
    for(int i = 1; i < depth; ++i) {
        // Condition when the depth
        // is exploring
        if(flag[i] == true) {
            printf("│   ");
        }
        // Otherwise print
        // the blank spaces
        else {
            printf("    ");
        }
    }

    // Condition when the current
    // node is the root node
    if(depth == 0) {
        printf("%d\n", (int)*ptr_node->data);
    } else if(isLast) {
        // Condition when the node is
        // the last node of
        // the exploring depth
        printf("└───%d\n", (int)*ptr_node->data);

        // No more childrens turn it
        // to the non-exploring depth
        flag[depth] = false;
    } else {
        printf("├───%d\n", (int)*ptr_node->data);
    }

    tree_node** pointer_to_children_pointers =
        (tree_node**)ptr_node->children.ptr_first_elem;
    tree_node* child_node_ptr = *pointer_to_children_pointers;

    for(size_t i = 0; i != ptr_node->children.dynarr_size; ++i) {
        // Recursive call for the
        // children nodes
        bool isLastChild = i == ptr_node->children.dynarr_size - 1;
        printTree(child_node_ptr, depth + 1, isLastChild, flag);

        child_node_ptr = *(++pointer_to_children_pointers);
    }
    flag[depth] = true;
}