
#include "LinkedListTests.h"
#include <stdlib.h>
#include "minunit.h"
#include "../LinkedList.h"

static char * test_create_linked_list() {

    LinkedList* linkedList = createLinkedList();

    mu_assert("Error: test_create_linked_list", linkedList->count == 0 && !linkedList->head && !linkedList->tail);

	return 0;
}


static char * test_append_to_new_linked_list() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");

    mu_assert("Error: test_append_to_new_linked_list", linkedList->count == 1 && linkedList->head == linkedList->tail
		&& strcmp(linkedList->head->key, "Key1") == 0 && strcmp(linkedList->head->value, "Value1") == 0  );

	return 0;
}


static char * test_append_operation() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");
	append(linkedList, "Key2", "Value2");

    mu_assert("Error: test_append_operation", linkedList->count == 2 && linkedList->tail == linkedList->head->next
		&& strcmp(linkedList->head->key, "Key1") == 0 && strcmp(linkedList->head->value, "Value1") == 0
		&& strcmp(linkedList->tail->key, "Key2") == 0 && strcmp(linkedList->tail->value, "Value2") == 0  );

	return 0;
}

static char * test_find_exist_node()  {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");
	append(linkedList, "Key2", "Value2");
	char* foundValue = find(linkedList, "Key1");

    mu_assert("Error: test_find_exist_node", linkedList->count == 2 && linkedList->tail == linkedList->head->next
		&& strcmp(foundValue, "Value1") == 0);

	return 0;
}




static char * test_find_non_exist_node() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");
	append(linkedList, "Key2", "Value2");

	char* foundValue = find(linkedList, "Key3");

    mu_assert("Error: test_find_non_exist_node", linkedList->count == 2 && linkedList->tail == linkedList->head->next
		&& foundValue == NULL);

	return 0;
}



static char * test_find_non_exist_node_in_empty_list() {

    LinkedList* linkedList = createLinkedList();

	char* foundValue = find(linkedList, "Key");

    mu_assert("Error: test_find_non_exist_node_in_empty_list", linkedList->count == 0 && linkedList->tail == linkedList->head
		&& linkedList->head == NULL	&& foundValue == NULL );

	return 0;

}


static char * test_free_linked_list() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");
	append(linkedList, "Key2", "Value2");

	freeLinkedList(&linkedList);

    mu_assert("Error: test_free_linked_list", linkedList == NULL);

	return 0;
}


static char * test_contains_duplicate() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");
	append(linkedList, "Key2", "Value2");
	append(linkedList, "Key3", "Value3");
	append(linkedList, "Key2", "Value93857");


    mu_assert("Error: test_contains_duplicate", containsDuplicate(linkedList) == true);

	freeLinkedList(&linkedList);

	return 0;
}


static char * test_does_not_contain_duplicate() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");
	append(linkedList, "Key2", "Value2");
	append(linkedList, "Key3", "Value3");


    mu_assert("Error: test_does_not_contain_duplicate", containsDuplicate(linkedList) == false);

	freeLinkedList(&linkedList);

	return 0;
}

static char * test_contain_duplicate_with_one_node_only() {

    LinkedList* linkedList = createLinkedList();
	append(linkedList, "Key1", "Value1");

    mu_assert("Error: test_contain_duplicate_with_one_node_only", containsDuplicate(linkedList) == false);

	freeLinkedList(&linkedList);

	return 0;
}


static char * test_contain_duplicate_with_no_node() {

    LinkedList* linkedList = createLinkedList();

    mu_assert("Error: test_contain_duplicate_with_no_node", containsDuplicate(linkedList) == false);

	freeLinkedList(&linkedList);

	return 0;
}

char * run_all_linked_list_tests() {

     mu_run_test(test_create_linked_list);
     mu_run_test(test_append_to_new_linked_list);
     mu_run_test(test_append_operation);
     mu_run_test(test_find_exist_node);
     mu_run_test(test_find_non_exist_node);
     mu_run_test(test_find_non_exist_node_in_empty_list);
     mu_run_test(test_free_linked_list);
     mu_run_test(test_contains_duplicate);
     mu_run_test(test_does_not_contain_duplicate);
     mu_run_test(test_contain_duplicate_with_one_node_only);
     mu_run_test(test_contain_duplicate_with_no_node);

     return 0;
}
