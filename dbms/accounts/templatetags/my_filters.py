import base64
from django import template

register = template.Library()

@register.filter
def base64encode(value):
    if isinstance(value, bytes):
        return base64.b64encode(value).decode('utf-8')
    else:
        return value
