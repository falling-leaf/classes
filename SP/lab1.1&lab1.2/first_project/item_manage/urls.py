from django.urls import path

from item_manage.views import ItemList, ItemCreate, ItemUpdate, ItemDelete

app_name = 'item_manage'


urlpatterns = [
    path('', ItemList.as_view(), name='item_list'),
    path('create/', ItemCreate.as_view(), name='item_create'),
    path('<int:pk>/update/', ItemUpdate.as_view(), name='item_update'),
    path('<int:pk>/delete/', ItemDelete.as_view(), name='item_delete'),
]
