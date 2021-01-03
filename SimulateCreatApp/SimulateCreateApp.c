#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAX_NAME_LEN     (32)
#define MAX_VERSION_LEN  (32)
#define MAX_LOCATION_LEN (128)

struct API
{
   char name[MAX_NAME_LEN];
   char version[MAX_VERSION_LEN];
   char location[MAX_LOCATION_LEN];
   int status;
};

typedef struct API APP;
typedef struct node* PNode;
//节点的定义
typedef struct node
{
	APP api;
	PNode next;
}Node,*SList;

Node *APPhead = NULL,*APPp = NULL;
int APPtotal = 0;

//初始化API
int APIGW_InitAPI()
{
	APPtotal = 0;
	APPhead = (Node*)malloc(sizeof(Node));
	if (APPhead == NULL)
		return 2;
	APPhead ->next = NULL;

	APPp = APPhead;
	return 0;
};

//导入API
int APIGW_ImportAPI(char *apiName, char *apiVersion, char *apiLocation)
{
	int i;
	Node *p = NULL, *q = NULL;

	if(NULL == apiName 
		|| NULL == apiVersion 
		|| NULL == apiLocation)
	{
		return 2;//错误
	}

	i = 0;
	p = APPhead;
	while(i < APPtotal)
	{
        if(0 == memcmp(p->next->api.name, apiName, MAX_NAME_LEN))
		{
			if(0 == memcmp(p->next->api.version, apiVersion, MAX_VERSION_LEN))
			{
				return 1;//已经存在
			}
			if(0 == memcmp(p->next->api.location, apiLocation, MAX_LOCATION_LEN))
			{
				return 2;//该路径已经存在API，错误
			}
		}
		i++;
		p = p->next;
	}

	q = (Node*)malloc(sizeof(Node));
	if (q == NULL)
	{
		return 2;
	}

	memcpy(q->api.name, apiName, MAX_NAME_LEN); 
	memcpy(q->api.version, apiVersion, MAX_VERSION_LEN); 
	memcpy(q->api.location, apiLocation, MAX_LOCATION_LEN); 
	q->api.status = 0;
	q->next = NULL;
    APPp->next = q;//链接起来
    APPp = q;//重新指向
	APPtotal += 1;//已经创建APP数加1
    printf("IMPORT:\t%d\t%s\t%s\t%s\n",APPtotal,apiName, apiVersion, apiLocation);

	return 0;
}
//删除API
int APIGW_DeleteAPI(char *apiName, char *apiVersion)
{
	int i;
	Node *p = NULL;

	if(NULL == apiName 
		|| NULL == apiVersion)
	{
		return 2;//错误
	}

	i = 0;
	p = APPhead;
    while(i < APPtotal)
	{
		if(0 == memcmp((p)->next->api.name, apiName, MAX_NAME_LEN))
		{
			if(0 == memcmp((p)->next->api.version, apiVersion, MAX_VERSION_LEN))
			{
				break;//找到对应APP
			}
		}
		i++;
		p = p->next;
	}

	if (i == APPtotal)
	{
		return 2;//不存在
	}

	//后节点覆盖当前节点，并删除后节点
	memcpy((p)->next, (p)->next->next, sizeof(Node));
	free((p)->next->next);
	APPtotal -= 1;

    printf("DELET:\t\t%s\t%s\n",apiName, apiVersion);
	return 0;
}

//发布API
int APIGW_PublishAPI(char *apiName, char *apiVersion)
{
    int i;
	Node *p = NULL;

	if(NULL == apiName 
		|| NULL == apiVersion)
	{
		return 2;//错误
	}

	i = 0;
	p = APPhead;
    while(i < APPtotal)
	{
		if(0 == memcmp((p)->next->api.name, apiName, MAX_NAME_LEN))
		{
			if(0 == memcmp((p)->next->api.version, apiVersion, MAX_VERSION_LEN))
			{
				break;//找到对应APP
			}
		}
		i++;
		p = p->next;
	}

	if (i == APPtotal)
	{
		return 2;//不存在
	}

	(p)->next->api.status = 1;//发布状态改为已经发布
    printf("PUBLIST:\t%s\t%s\tSTATUS: %d\n",apiName, apiVersion,(p)->next->api.status);
	return 0;
}
//取消发布API
int APIGW_UnPublishAPI(char *apiName, char *apiVersion)
{
	int i;
	Node *p = NULL;

	if(NULL == apiName 
		|| NULL == apiVersion)
	{
		return 2;//错误
	}

	i = 0;
	p = APPhead;
    while(i < APPtotal)
	{
		if(0 == memcmp((p)->next->api.name, apiName, MAX_NAME_LEN))
		{
			if(0 == memcmp((p)->next->api.version, apiVersion, MAX_VERSION_LEN))
			{
				break;//找到对应APP
			}
		}
		i++;
		p = p->next;
	}

	if (i == APPtotal)
	{
		return 2;//不存在
	}

	if ((p)->next->api.status == 1)
	{
		(p)->next->api.status = 0;
	}
    printf("UNPUBLIST:\t%s\t%s\tSTATUS: %d\n",apiName, apiVersion,(p)->next->api.status);
	return 0;
}

//获取API状态
int APIGW_GetAPIStaus(char *apiName, char *apiVersion, int *apiStatus)
{
	int i;
	Node *p = NULL;

	if(NULL == apiName 
		|| NULL == apiVersion)
	{
		return 2;//错误
	}

	i = 0;
	p = APPhead;
    while(i < APPtotal)
	{
		if(0 == memcmp((p)->next->api.name, apiName, MAX_NAME_LEN))
		{
			if(0 == memcmp((p)->next->api.version, apiVersion, MAX_VERSION_LEN))
			{
				break;//找到对应APP
			}
		}
		i++;
		p = p->next;
	}

	if (i == APPtotal)
	{
		return 2;//不存在
	}

	*apiStatus = (p)->next->api.status;
    printf("GetStatus:\t%s\t%sSTATUS:%d\n",apiName, apiVersion,(p)->next->api.status);
	return 0;
}

//获取API保存位置
int APIGW_GetAPILocation(char *apiName, char *apiVersion, int *apiLocation)
{
	int i;
	Node *p = NULL;

	if(NULL == apiName 
		|| NULL == apiVersion)
	{
		return 2;//错误
	}

	i = 0;
	p = APPhead;
    while(i < APPtotal)
	{
		if(0 == memcmp((p)->next->api.name, apiName, MAX_NAME_LEN))
		{
			if(0 == memcmp((p)->next->api.version, apiVersion, MAX_VERSION_LEN))
			{
				break;//找到对应APP
			}
		}
		i++;
		p = p->next;
	}

	if (i == APPtotal)
	{
		return 2;//不存在
	}

	memcpy(apiLocation, (p)->next->api.location, MAX_LOCATION_LEN);
    printf("GetLocation:\t%s\t%sLOCATION:%s\n",apiName, apiVersion,(p)->next->api.location);
	return 0;
}

int main(int argc, char const *argv[])
{
	/* code */
	int result = 0;

	result = APIGW_InitAPI();
	result = APIGW_ImportAPI("ABC", "1.0", "C:/a/a");
    result = APIGW_ImportAPI("ABC", "1.1", "C:/a/a/c");
	result = APIGW_ImportAPI("ABCD", "1.0", "C:/a/a/d");
	result = APIGW_ImportAPI("ABCDE", "1.0", "C:/a/a/e");
	result = APIGW_ImportAPI("ABCDEF", "1.0", "C:/a/a/f");

    APIGW_PublishAPI("ABC", "1.0");
	APIGW_UnPublishAPI("ABC", "1.0");
	APIGW_DeleteAPI("ABC", "1.0");
	while(1);
}


