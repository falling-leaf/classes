from django import forms

from item_manage.models import Item


class ItemCreateForm(forms.ModelForm):

    class Meta:

        model = Item
        fields = '__all__'

        widgets = {
            'name': forms.TextInput(attrs={'id': 'name', 'class': 'form-control'}),
            'type': forms.TextInput(attrs={'id': 'type', 'class': 'form-control'}),
            'price': forms.NumberInput(attrs={'id': 'price', 'class': 'form-control', 'step': '0.1'}),
        }

        labels = {
            'name': '商品名称',
            'type': '类型',
            'price': '价格',
        }


class ItemUpdateForm(ItemCreateForm):
    pass
