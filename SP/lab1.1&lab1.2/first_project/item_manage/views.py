# Create your views here.
from django.urls import reverse, reverse_lazy
from django.views.generic import ListView, CreateView, UpdateView, DeleteView

from item_manage.forms import ItemCreateForm, ItemUpdateForm
from item_manage.models import Item


class ItemList(ListView):
    model = Item
    template_name = 'item_manage/item_list.html'


class ItemCreate(CreateView):
    form_class = ItemCreateForm
    model = Item
    template_name = 'item_manage/item_create.html'
    success_url = reverse_lazy('item_manage:item_list')

class ItemUpdate(UpdateView):
    model = Item
    form_class = ItemUpdateForm
    template_name = 'item_manage/item_update.html'
    success_url = reverse_lazy('item_manage:item_list')


class ItemDelete(DeleteView):
    model = Item
    template_name = 'item_manage/item_delete.html'
    success_url = reverse_lazy('item_manage:item_list')