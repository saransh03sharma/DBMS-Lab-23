#include "defs.h"

Page::Page():pin(false),ref_bit(false),fptr(NULL),page_num(-1){
	disk_block = malloc(PAGE_SIZE);
}

Page::~Page(){
	free(disk_block);
}

clock_buffer_manager::clock_buffer_manager(unsigned int n):
num_bufs(n),clock_hand(n-1),accesses(0),disk_reads(0),buf_cnt(0){
	buf_pool = new Page[n];
}

clock_buffer_manager::~clock_buffer_manager(){
	delete[] buf_pool;
}

void clock_buffer_manager::unpin_page(Page* p){
	p->pin = false;
}

//clock replacement algorithm
int clock_buffer_manager::replace_page(FILE*f,int page_number){
	for(int i=0;i<num_bufs;i++){
		clock_hand = (clock_hand + 1)%num_bufs;
		if(buf_pool[clock_hand].ref_bit == 1){
			buf_pool[clock_hand].ref_bit = 0;
			continue;
		}
		else{
			if(buf_pool[clock_hand].pin == 1){
				continue;
			}
			else{
				int r = fseek(f,page_number*PAGE_SIZE,SEEK_SET);
				if(r<0)return -1;
				r = fread(buf_pool[clock_hand].disk_block,PAGE_SIZE,1,f);
				if(r!=1)return -1;
				return clock_hand;
			}
		}
	}
	return -1;
}

//zero-indexed page number
//returns NULL on error
Page *clock_buffer_manager::read_page(FILE *f,int page_number){
	if(f==NULL || page_number<0)return NULL;
	pair <FILE *,int> key = make_pair(f,page_number);
	int idx;
	if(buf_map.find(key) != buf_map.end()){
		accesses++;
		idx = buf_map[key];
		buf_pool[idx].pin = buf_pool[idx].ref_bit = 1;
	}
	else if(buf_cnt<num_bufs){
		int r = fseek(f,page_number*PAGE_SIZE,SEEK_SET);
		if(r<0)return NULL;
		r = fread(buf_pool[buf_cnt].disk_block,PAGE_SIZE,1,f);
		if(r!=1)return NULL;
		disk_reads++;
		idx = buf_cnt;
		buf_map[key] = idx;
		buf_pool[idx].pin = buf_pool[idx].ref_bit = 1;		
		buf_pool[idx].fptr = f;
		buf_pool[idx].page_num = page_number;
		buf_cnt++;
	}
	else{
		idx = replace_page(f,page_number);
		if(idx==-1)return NULL;
		disk_reads++;
		auto it = make_pair(buf_pool[idx].fptr,buf_pool[idx].page_num);
		buf_map.erase(it);
		buf_map[key] = idx;
		
		buf_pool[idx].pin = buf_pool[idx].ref_bit = 1;
		buf_pool[idx].fptr = f;
		buf_pool[idx].page_num = page_number;		
	}
	return &(buf_pool[idx]);
}

lru_buffer_manager::lru_buffer_manager(unsigned int n):
num_bufs(n), accesses(0), disk_reads(0),buf_cnt(0){
	buf_pool.clear();
}

lru_buffer_manager::~lru_buffer_manager(){
	for(auto page:buf_pool){
		if(page!=NULL)delete page;
	}
}

void lru_buffer_manager::unpin_page(Page* p){
	p->pin = false;
	pair <FILE*,int> key = make_pair(p->fptr,p->page_num);
	list<Page*>::iterator it = buf_map[key];
	buf_pool.erase(it);
	buf_map.erase(key);
	
	buf_pool.push_back(p);
	auto it2 = buf_pool.end();
	it2--;
	buf_map[key] = it2;
	return;
}

Page* lru_buffer_manager::read_page(FILE *fptr,int page_number){
	if(fptr==NULL || page_number<0)return NULL;
	pair<FILE *, int> key = make_pair(fptr,page_number);
	if(buf_map.find(key)!=buf_map.end()){
		accesses++;
		list <Page*>::iterator it = buf_map[key];
		Page *p = *it;
		p->pin = 1;
		buf_pool.erase(it);

		buf_pool.push_front(p);
		buf_map[key] = buf_pool.begin();
		return p;
	}
	else if(buf_cnt<num_bufs){
		int r = fseek(fptr,page_number*PAGE_SIZE,SEEK_SET);
		if(r<0)return NULL;
		Page *x = new Page;
		x->fptr = fptr;
		x->page_num = page_number;
		x->pin = 1;
		r = fread(x->disk_block,PAGE_SIZE,1,fptr);
		if(r!=1){
			delete x;
			return NULL;
		}
		buf_cnt++;	
		buf_pool.push_front(x);
		buf_map[key] = buf_pool.begin();
		disk_reads++;
		return x;
	}
	else{
		list <Page*>::iterator it = buf_pool.end();
		it--;
		Page *x = *it;
		if(x->pin == 1)return NULL;
		
		int r = fseek(fptr,page_number*PAGE_SIZE,SEEK_SET);
		if(r<0)return NULL;
		buf_map.erase(make_pair(x->fptr,x->page_num));
		buf_pool.erase(it);
		
		x->pin = 1;
		x->fptr = fptr;
		x->page_num = page_number;
		fread(x->disk_block,PAGE_SIZE,1,fptr);
		
		buf_pool.push_front(x);
		buf_map[make_pair(x->fptr,x->page_num)] = buf_pool.begin();
		
		disk_reads++;	
		return x;
	}
}

