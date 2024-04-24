short degree;
typedef struct bp_tree bp_tree;
typedef struct node node;

typedef struct data {
	int id;
	void* db_ptr;
}data;

typedef struct node {
	data** data_arr;
	int data_index;
	node* next;
}node;

struct bp_tree {
	node* arr_node;
	bp_tree** bp_tree_arr;
	int bp_tree_index;
	unsigned short bp_tree_height;
};

typedef struct backup {
	int new_id;
	node* next;
	int left_height, right_height;
}backup;

void create_bp_tree(bp_tree** tree);
void create_node(bp_tree** tree);
void grow_data_array(bp_tree** tree, int size);
void grow_data_array2(data*** temp, int* index, int size);
void create_data(bp_tree** tree, int i, int id);
void shift_left_data_arr(bp_tree** tree, int start);
void shift_left_data_arr2(data*** data_arr, int start_index, int end_index);
void shift_right_data_arr(bp_tree** tree, int end);
void grow_bp_tree_array(bp_tree** tree, int size);
void shrink_data_array(bp_tree** tree, int size);
int find_input_index(bp_tree** tree, int id);
void shift_right_bp_tree_arr(bp_tree** tree, int end);
void shift_left_bp_tree_arr(bp_tree** tree, int start, int cp_index);
void copy_bp_tree_array(bp_tree** c_tree, int start, int end, bp_tree** m_tree);
void copy_data_arr(bp_tree** c_tree, int start, int end, bp_tree** m_tree);
void copy_data_arr2(data*** dst_data_arr, int start_index, int end_index, data*** scr_data_arr);
void shrink_bp_tree_arr(bp_tree** tree, int size);
void split_bp_tree(bp_tree** root, bp_tree** tree);
void insert_node(bp_tree** root, bp_tree** tree, int id);
void insert(bp_tree** tree, int id);
int height(bp_tree* root, int i);
void free_bp_tree(bp_tree** root);
void copy_arr_node(bp_tree** root, int start);
void height_shrink(bp_tree** root, int i, short LR);
void remove_id(bp_tree** root, int id);
void delete_id(bp_tree** root, int id);
void print_id(bp_tree* temp);
bp_tree* dummy_tree(void);
bp_tree* get_smallest_node(bp_tree*);
bp_tree* get_greatest_node(bp_tree*);