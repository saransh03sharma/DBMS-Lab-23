from django.contrib import admin
from .models import *

# admin.site.register(User)#register a model to get is displayed at admin's portal
# admin.site.register(Student)
# admin.site.register(Alumni)
# admin.site.register(Company)
# admin.site.register(Chat)
# admin.site.register(Notification)

admin.site.register(front_desk)
admin.site.register(data_entry)
admin.site.register(physician)