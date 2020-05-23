#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

// TRIE

#define CHAR_SIZE 26
struct Trie
{
	int isLeaf;	// 1 when node is a leaf node
	struct Trie* character[CHAR_SIZE];
};

// Funçao que retorna um novo no Trie
struct Trie* getNewTrieNode()
{
	struct Trie* node = (struct Trie*)malloc(sizeof(struct Trie));
	node->isLeaf = 0;
    int i;
	for (i = 0; i < CHAR_SIZE; i++)
		node->character[i] = NULL;

	return node;
}

// Funçao iterativa para inserir uma string na trie
void insert(struct Trie* *head, char* str)
{
	// Começa da raiz
	struct Trie* curr = *head;
	while (*str)
	{

		if (curr->character[*str - 'a'] == NULL)
			curr->character[*str - 'a'] = getNewTrieNode();
		curr = curr->character[*str - 'a'];
		str++;
	}

	// marca que o no eh folha
	curr->isLeaf = 1;
}

// Funçao iterativa para buscar uma palavra. Retorna 1 se existir
int searchTrie(struct Trie* head, char* str)
{
	// return 0 se Trie esta vazia
	if (head == NULL)
		return 0;

	struct Trie* curr = head;
	while (*str)
	{
		curr = curr->character[*str - 'a'];
		if (curr == NULL)
			return 0;
		str++;
	}
	return curr->isLeaf;
}
int haveChildren(struct Trie* curr)
{
    int i;
	for (i = 0; i < CHAR_SIZE; i++)
		if (curr->character[i])
			return 1;	// child found

	return 0;
}

// Funçao recursiva para deletar uma string
int deletion(struct Trie* *curr, char* str)
{

	if (*curr == NULL)
		return 0;
	if (*str)
	{

		if (*curr != NULL && (*curr)->character[*str - 'a'] != NULL &&
			deletion(&((*curr)->character[*str - 'a']), str + 1) &&
			(*curr)->isLeaf == 0)
		{
			if (!haveChildren(*curr))
			{
				free(*curr);
				(*curr) = NULL;
				return 1;
			}
			else {
				return 0;
			}
		}
	}

	if (*str == '\0' && (*curr)->isLeaf)
	{

		if (!haveChildren(*curr))
		{
			free(*curr);
			(*curr) = NULL;
			return 1;
		}


		else
		{

			(*curr)->isLeaf = 0;
			return 0;
		}
	}

	return 0;
}


#ifndef __LIST_H
#define __LIST_H

#define offset_of(type,member) ((size_t)&(((type*)0)->member))

#define LIST_POISON1	0xdeaddead
#define LIST_POISON2	0xbedeadbe

#define container_of(pointer,type,member)\
(type*)((char*)(pointer) - offset_of(type,member))

struct list_head {
        struct list_head *prev,*next;
};

static inline void INIT_LIST_HEAD(struct list_head *list)
{
        list->next = list;
        list->prev = list;
}

static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}
static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
        __list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        entry->next = LIST_POISON1;
        entry->prev = LIST_POISON2;
}

static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

static inline int list_empty(const struct list_head *head)
{
        return head->next == head;
}

static inline void list_del_init(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        INIT_LIST_HEAD(entry);
}

#define list_for_each_safe(pos, n, head) \
        for (pos = (head)->next, n = pos->next; pos != (head); \
                pos = n, n = pos->next)

#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)

#define INIT_PTRIE_ROOT(ROOT)\
	INIT_LIST_HEAD(& ((ROOT)->next))

#endif 


struct ptrie_node {
	struct list_head next;
	struct list_head sibling_list;
	char	*substr;
};

struct ptrie_root {
	struct list_head next;
};

static struct ptrie_node* new_node(struct ptrie_node *node,int sibling)
{
	struct list_head *list;
	struct ptrie_node *new_node;
	if(!node) {
		new_node = calloc(1,sizeof(struct ptrie_node));
		if(!new_node)
			return NULL;
		INIT_LIST_HEAD(&new_node->next);
		INIT_LIST_HEAD(&new_node->sibling_list);
		return new_node;
	}
	if(sibling)
		list = &node->sibling_list;
	else
		list = &node->next;
	new_node = calloc(1,sizeof(struct ptrie_node));
	if (!new_node) {
		return NULL;
	}
	INIT_LIST_HEAD(&new_node->next);
	INIT_LIST_HEAD(&new_node->sibling_list);
	list_add_tail(&new_node->sibling_list,list);
return new_node;
}

static int __add_word(struct ptrie_node *node,const char *word)
{

	if (node->substr && strlen(word)<=strlen(node->substr)) {
		strcpy(node->substr,word);
		return;
	}
	if (node->substr) {
		free(node->substr);
		node->substr = NULL;
	}
	node->substr = calloc(1,strlen(word)+1);
	if (!node->substr)
		return ENOMEM;
	strcpy(node->substr,word);
return 0;
}

static int split_trie(struct ptrie_node *node,ssize_t offset)
{
	struct ptrie_node *nnode = new_node(node,0);
	char *substr;
	if (!nnode)
		return ENOMEM;
	if (__add_word(nnode,node->substr+offset)) {
		free_node(nnode);
		return ENOMEM;
	}
	*(node->substr+offset) = '\0';
	substr = calloc(1,strlen(node->substr)+1);
	if (!substr)
		return ENOMEM;
	strcpy(substr,node->substr);
	free(node->substr);
	node->substr = substr;
return 0;
}


void free_node(struct ptrie_node *node)
{
	struct list_head *cur,*next;
	list_for_each_safe(cur,next,&node->next) {
		struct ptrie_node *item = list_entry(cur,struct ptrie_node
					,sibling_list);
		free_node(item);
		list_del_init(&item->sibling_list);
		if(item->substr)
			free(item->substr);
		free(item);
	}
	if(node->substr)
		free(node->substr);
	list_del_init(&node->sibling_list);
	free(node);
}

static int __match_index(const char *str1,const char *str2)
{
	int indx = 0;
	while (*str1 && *str2) {
		if(*str1++ == *str2++)
			indx++;
		else	break;
	}
	return indx;
}

static struct ptrie_node* __search(struct ptrie_root *root,const char *word,
					int *status)
{
	struct list_head *cur,*next;
	struct list_head *tmp_head = &root->next;
	off_t offset = 0;
	struct ptrie_node *item=NULL;
	struct ptrie_node *most_matched = NULL;
	int last_most_matched_index = 0,cur_match_indx = 0,substr_len;
	*status = 0;
search_again:
	list_for_each_safe(cur,next,tmp_head) {
		item = list_entry(cur,struct ptrie_node,sibling_list);
		if (item->substr) {
			substr_len = strlen(word)>strlen(item->substr)?
					strlen(item->substr):strlen(word);

			if(!strncmp(word + offset,item->substr,substr_len)) {
				last_most_matched_index = 0;
				most_matched = item;
				offset += substr_len;
				if(offset == strlen(word)) {
					*status = 1;
					goto done;
				}
				else { 
					tmp_head = &item->next;
					goto search_again;
				}
			}
			else {
				if (last_most_matched_index <
					(cur_match_indx =
					__match_index(word+offset,item->substr)
						)) {
						last_most_matched_index =
							cur_match_indx;
						most_matched = item;
				}
			}
		}
	}
	return most_matched;
done:
	return item;
}
int search(struct ptrie_root *root,const char *word)
{
	int found;
	if(!word) return 0;
	__search(root,word,&found);
	return found;
}

int add_word(struct ptrie_root *root,const char *word)
{
	int found;
	struct ptrie_node *split_node = __search(root,word,&found);
	struct ptrie_node *nnode = NULL;
	int offset = 0;
	if (!found) {
		if(split_node) {
			offset = __match_index(word,split_node->substr);
			if(split_trie(split_node,offset))
				return ENOMEM;
		}
		nnode = new_node(split_node,0);
		if (!nnode)
			return ENOMEM;
		if(!split_node)
			list_add_tail(&nnode->sibling_list,&root->next);
		__add_word(nnode,word+offset);
	}
	return 0;
}

/*
    Define uma forma de limpar a tela do terminal,
    tanto no Linux quanto no Windows.
 */
#ifdef __linux__
    #define CLEAR() (system("clear"))
#elif _WIN32
    #define CLEAR() (system("cls"))
#endif
#define MSG_LEN 100

/*
    Imprime uma mensagem de cabeçalho
    compartilhada por todas as screens
 */
void print_header();

/*
    Tela de Home
    Permite acessar a screen de criação de B-Tree e a screen Sobre.
    Permite sair da aplicação.
 */
int home_screen();
/*
    Informações básicas sobre o projeto B-Tree e essa DEMO
 */
int about_screen();
/*
    Tela para definir a ordem da B-Tree a ser criada
 */
int run_screen();
/*
    Tela onde as operações INSERÇÃO, REMOÇÃO, PESQUISA, IMPRESSÃO serão executadas
 */
int running_screen();
/*
    Função auxiliar para gerar uma mensagem de "despedida"
 */
void goodbye();
/*
    A ordem das 3 primeiras screens deve ser RUN, ABOUT, EXIT.
    Os valores atribuídos à elas são utilizados na lógica interna do menu.
 */
typedef enum {
	RUN, ABOUT, EXIT, HOME, RUNNING
} screen;

/*
    Indica uma mensagem que será exibida no topo da próxima screen.
    É uma forma de comunicar mensagens ao usuário.
    O valor default (quando não há mensagens para exibir) deve ser o caracter '-'.
 */
char msg[MSG_LEN];

struct ptrie_root proot;
struct Trie* head;
/*
    Contador de quantas keys foram inseridas
 */
int n_keys;

int tipoarv;

int home_screen() {
	int opt;
	int n_items = 4;
	// Itens no menu
	char list[4][12] = { "Patricia", "Trie", "Sobre","Sair" };

	print_header();

	int i;
	for (i = 0; i < n_items; ++i) {
		printf(" %d) %s\n", i+1, list[i]);
	}
	printf("\n");
	printf(" Digite uma opcao: ");
	scanf("%d%*c", &opt);

	if(opt==3)
		return ABOUT;
	
	if(opt==4)
		return 3;

	tipoarv=opt;
	if (opt < 1 || opt > n_items) {
		snprintf(msg, MSG_LEN, " Opcao invalida.");
		return HOME;
	}

	return 0;
}

int about_screen() {
	print_header();
	printf( "---------------------------------------------------------\n" \
	        "--       Trabalho apresentado na disciplina AED2       --\n" \
	        "--            ministrada pelo Prof. Edson              --\n" \
	        "--                    UFAM                             --\n" \
	        "---------------------------------------------------------\n");
	printf("\n");

	printf(" Pressione ENTER para voltar ao menu. ");
	getchar();

	return HOME;
}

int run_screen() {
	print_header();

	int order;

	printf("...Iniciando ARVORE");

	printf("\n");
	printf(" Pressione ENTER para continuar. ");
	getchar();
	// Única criação de uma Patricia
	INIT_PTRIE_ROOT(&proot);
	// Unica criaçao de uma Trie
	head = getNewTrieNode();

	return RUNNING;
}

int running_screen() {
	int n_items;
	char xxx[3][15] = { "Inserir", "Buscar","Remover"};
	n_items=3;
	if(tipoarv==1){
	n_items=2;
     }

	int opt;

	print_header();

	int i;
	for (i = 0; i < n_items; ++i) {
		printf(" %d) %s\n", i+1, xxx[i]);
	}
	printf(" 0) Sair");
	printf("\n");
	printf(" Digite uma opcao: ");
	scanf("%d%*c", &opt);
	printf("\n");
	int op;
	char palv[25];
	switch (opt) {
	case 1:                 // Inserir
			printf(" Digite uma palavra:");
			scanf("%s",palv);
			if(tipoarv)
				add_word(&proot,palv);
			else insert(&head, palv);
			snprintf(msg, MSG_LEN, " Insercao realizada com sucesso.");
			n_keys++;
			break;
	case 2:                 // Consultas
			 printf(" Digite uma palavra: ");
			scanf("%s",palv);
			if(tipoarv){
					if(search(&proot,palv)==1)
						printf("A palavra %s esta contida na arvore\n",palv);
					else
						printf("A palavra %s nao esta contida na arvore\n",palv);
				}
			else {
				if(searchTrie(head, palv)){
					printf("A palavra %s esta contida na arvore\n",palv);
				}
				else printf("A palavra %s nao esta contida na arvore\n",palv);
			}
					printf(" Pressione ENTER para continuar. ");
					getchar();
					getchar();
		break;
	case 3:                 //Remover
			printf(" Digite uma palavra: ");
			scanf("%s",palv);
			if(searchTrie(head, palv)){
					deletion(&head, palv);
					printf("A palavra %s foi removida\n",palv);
				}
			else printf("A palavra %s nao esta na arvore\n",palv);
			printf(" Pressione ENTER para continuar. ");
			getchar();
			getchar();
		break;
	case 0:                 // Voltar/Sair
		snprintf(msg, MSG_LEN, " Voltando ao menu");
		goodbye();
		return HOME;
		break;
	default:
		snprintf(msg, MSG_LEN, " Opcao invalida");
		return RUNNING;
		break;
	}

	return RUNNING;
}

void print_header() {
	CLEAR();
	printf( "---------------------------------------------------------\n" \
	        "--         	Arvore Trie e Arvore Patricia          --\n" \
	        "--                  Desenvolvido por:                  --\n" \
	        "--      Jean Cleison Braga Guimaraes - 21601227        --\n" \
	        "---------------------------------------------------------\n");

	printf("\n");

	if (strcmp(msg, "-")) {
		printf("%s\n\n", msg);
		snprintf(msg, MSG_LEN, "-");
	}
}

void goodbye() {
	// Apenas uma animação simples de "loading"
	print_header();
	int i;
	printf(" ");
	for (i = 0; i < 4; ++i) {
		printf(".");
		fflush(stdout);
		usleep(500000);
	}

	CLEAR();
}

int main() {
	screen next_screen = HOME;
	snprintf(msg, MSG_LEN, "-");
	n_keys = 0;

	while (next_screen != EXIT) {
		switch (next_screen) {
		case HOME:
			next_screen = home_screen();
			break;
		case ABOUT:
			next_screen = about_screen();
			break;
		case RUN:
			next_screen = run_screen();
			break;
		case RUNNING:
			next_screen = running_screen();
			break;
		case EXIT:
			break;
		}
	}

	snprintf(msg, MSG_LEN, " Saindo da aplicacao");
	goodbye();

	return 0;
}
