#all urls of the web application is specified here
# name=" "; this name will be use for linking withing HTML
#views.func() indicates that func() defined in views will be called once the url is opened 

from django.urls import path
from .import  views
from django.urls import path

urlpatterns=[
     path('register/',views.register, name='register'),#path(url,function name, name of link)
     path('doctor_register/',views.doctor_register.as_view(), name='doctor_register'),
     path('front_desk_register/',views.front_desk_register.as_view(), name='front_desk_register'),
     path('data_entry_register/',views.data_entry_register.as_view(), name='data_entry_register'),
     path('patient_register/',views.patient_reg_help.as_view(), name='patient_reg'),
    path('admit_discharge/',views.handle_admit, name='handle_admit'),
    path('admission/',views.admit_patient.as_view(), name='patient_admit'),
    #path('test_result/',views.handle_test, name='handle_test'),
    
    
    #  path('company_edit_details/',views.editCompProfile.as_view(), name='company_edit'),
    #  path('alumni_edit_details/',views.editAlumProfile.as_view(), name='alumni_edit'),
    #  path('request_feedback/',views.request_feedback, name='request_feedback'),
    #  path('feedback/',views.feedback, name='feedback'),
    
    #  path('company_details/',views.company_details, name='company_details'),
    #  path('stud_chat/',views.stud_chat, name='stud_chat'),
    #  path('chats/',views.chats, name='chats'),
    #  path('list_of_students/',views.list_of_students, name='list_of_students'),
    #  path('get_student_cv/',views.get_stud_cv, name='get_stud_cv'),
    #  path('get_cv/',views.get_cv, name='get_cv'),
     path('',views.index, name='index'),
     path('admin_login/',views.login_admin, name='admin_login'),
     path('doctor_login/',views.login_doctor, name='doctor_login'),
     path('fr_login/',views.login_fr, name='fr_login'),
     path('de_login/',views.login_de, name='de_login'),
     path('logout/',views.logout_view, name='logout'),
]
