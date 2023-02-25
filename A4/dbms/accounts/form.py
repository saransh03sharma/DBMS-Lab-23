import json
from django import forms
from django.db import transaction
from .models import *
from django.core.exceptions import ValidationError
from django.utils.translation import gettext_lazy as _
from django.contrib.auth.hashers import make_password, check_password


# Dep_choices = (# tuple of (what appears in the backend,what appears in frontend)
#     ("Computer Science and Engineering","Computer Science and Engineering"),
#     ("Electronics and Electrical Communications Engineering","Electronics and Electrical Communications Engineering"),
#     ("Electrical Engineering","Electrical Engineering"),
#     ("Mechanical Engineering","Mechanical Engineering"),
# )

patient_status =(
    ("0","Register"),
    ("1","Admitted"),
    ("2","Discharged"),
)

# Profiles_edit_choices =(
#     ("NO","No new CV to upload"),
#     ("SD","Software Develepment"),
#     ("DA","Data Analytics"),
#     ("DL","Delete my uploaded CVs"),
# )

class DoctorSignUpForm(forms.ModelForm):#form and formfields defined
   
    EmployeeID = forms.IntegerField(required=True)
    name =forms.CharField(required=True)
    Position =forms.CharField(required=True)
    SSN = forms.IntegerField(required=True)
    password = forms.CharField(required=True, widget=forms.PasswordInput)

    class Meta(forms.ModelForm):#Model Meta is basically used to change the behavior of your model fields like changing order options,verbose_name and lot of other options.
        model = physician
        # Order of Fields in the Form
        fields = ['EmployeeID','name','Position','SSN', 'password']
    
    def clean_strings(self,*args,**kwargs):
        name = self.cleaned_data.get('name').upper()#get the data from form which would be stored in self.cleaned and store it in upper case
        Position = self.cleaned_data.get('Position').upper() #extract email from form
        EmployeeID = self.cleaned_data.get('EmployeeID')
        SSN = self.cleaned_data.get('SSN')
        # print(email)


    @transaction.atomic  #if an exception occurs changes are not saved
    def save(self):

        name = self.cleaned_data.get('name').upper()#get the data from form which would be stored in self.cleaned and store it in upper case
        Position = self.cleaned_data.get('Position').upper() #extract email from form
        EmployeeID = self.cleaned_data.get('EmployeeID')
        password = make_password(self.cleaned_data.get('password'))
        
        SSN = self.cleaned_data.get('SSN')
        doctor = physician(name=name,Position = Position, EmployeeID = EmployeeID, SSN = SSN, password=password)
        doctor.save()
        return doctor


class FrontSignUpForm(forms.ModelForm):#form and formfields defined
   
    
    name =forms.CharField(required=True)
    surname =forms.CharField(required=True)
    reg_id = forms.IntegerField(required=True)
    password = forms.CharField(required=True, widget=forms.PasswordInput)
    

    class Meta(forms.ModelForm):#Model Meta is basically used to change the behavior of your model fields like changing order options,verbose_name and lot of other options.
        model = front_desk
        # Order of Fields in the Form
        fields = ['name', 'surname', 'reg_id', 'password']
    
    def clean_strings(self,*args,**kwargs):
        name = self.cleaned_data.get('name').upper()#get the data from form which would be stored in self.cleaned and store it in upper case
        surname = self.cleaned_data.get('surname').upper() #extract email from form
        reg_id = self.cleaned_data.get('reg_id')
        
        # print(email)


    @transaction.atomic  #if an exception occurs changes are not saved
    def save(self):

        name = self.cleaned_data.get('name').upper()#get the data from form which would be stored in self.cleaned and store it in upper case
        surname = self.cleaned_data.get('surname').upper() #extract email from form
        reg_id = self.cleaned_data.get('reg_id')
        password = make_password(self.cleaned_data.get('password'))
        front = front_desk(name=name,surname = surname, reg_id = reg_id, password=password)
        front.save()
        return front
    
class DataSignUpForm(forms.ModelForm):#form and formfields defined
    
    name =forms.CharField(required=True)
    surname =forms.CharField(required=True)
    reg_id = forms.IntegerField(required=True)
    password = forms.CharField(required=True, widget=forms.PasswordInput)

    class Meta(forms.ModelForm):#Model Meta is basically used to change the behavior of your model fields like changing order options,verbose_name and lot of other options.
        model = data_entry
        # Order of Fields in the Form
        fields = ['name', 'surname', 'reg_id', 'password']
    
    def clean_strings(self,*args,**kwargs):
        name = self.cleaned_data.get('name').upper()#get the data from form which would be stored in self.cleaned and store it in upper case
        surname = self.cleaned_data.get('surname').upper() #extract email from form
        reg_id = self.cleaned_data.get('reg_id')
        
        
        # print(email)


    @transaction.atomic  #if an exception occurs changes are not saved
    def save(self):

        name = self.cleaned_data.get('name').upper()#get the data from form which would be stored in self.cleaned and store it in upper case
        surname = self.cleaned_data.get('surname').upper() #extract email from form
        reg_id = self.cleaned_data.get('reg_id')
        password = make_password(self.cleaned_data.get('password'))
        data = data_entry(name=name,surname = surname, reg_id = reg_id, password=password)
        data.save()
        return data


class patient_register(forms.ModelForm):
    SSN = forms.IntegerField(required=True)
    name = forms.CharField(max_length = 255,required=True)
    Address = forms.CharField(max_length = 255,required=True)
    Phone = forms.CharField(max_length = 255,required=True)
    InsuranceID = forms.IntegerField(required=True)
    PCP = forms.IntegerField(required=True)
    Status = forms.ChoiceField(choices=patient_status)
#     email = forms.EmailField(required=True)
#     contact_number = forms.CharField(max_length=12,required=True)
#     roll_number =forms.CharField(required=True)
#     first_name = forms.CharField(required=True) 
#     last_name = forms.CharField(required=True)
#     department = forms.CharField(required=True)
#     SDprofile = forms.BooleanField(initial=True,label="Software Development",required=False)
#     DAprofile = forms.BooleanField(initial=False,label="Data Analytics",required=False)
#     cvprof = forms.ChoiceField(choices=Profiles_edit_choices,label="Choose a Profile to Upload CV for(choose No File to Upload, if don't want to upload new CV)",required=True)
#     cv = forms.FileField(label="Upload CV",required=False)

    class Meta():
        model = patient
        fields = ['SSN','name','Address','Phone','InsuranceID','PCP','Status']


    @transaction.atomic  #if an exception occurs changes are not saved
    def save(self):
        return self.cleaned_data.get('SSN'),self.cleaned_data.get('name'),self.cleaned_data.get('Address'),self.cleaned_data.get('Phone'),self.cleaned_data.get('InsuranceID'),self.cleaned_data.get('PCP'),self.cleaned_data.get('Status')

# class AlumniSignUpForm(UserCreationForm):
#     first_name = forms.CharField(required=True) 
#     last_name = forms.CharField(required=True)
#     department = forms.ChoiceField(choices=Dep_choices)
#     roll_number =forms.CharField(required=True)
#     year_of_graduation = forms.IntegerField(required=True,min_value=1951,max_value=2021)

#     class Meta(UserCreationForm.Meta):
#         model = User
#         fields = ['email','contact_number','roll_number','first_name','last_name','department','year_of_graduation']
    
#     @transaction.atomic#if an exception occurs changes are not saved
#     def save(self):
#         user = super().save(commit=False)#before saving save the details
#         user.is_alumni = True
#         user.first_name = self.cleaned_data.get('first_name')
#         user.last_name = self.cleaned_data.get('last_name')
#         user.contact_number=self.cleaned_data.get('contact_number')
#         user.email = self.cleaned_data.get('email')
#         user.username = user.email
#         user.save()
#         alumni = Alumni.objects.create(user=user)#instance created
#         alumni.department=self.cleaned_data.get('department')
#         alumni.roll_number=self.cleaned_data.get('roll_number').upper()
#         alumni.year_of_graduation=int(self.cleaned_data.get('year_of_graduation'))
#         alumni.save()
#         return user

# class AlumniEditForm(forms.ModelForm):
#     email = forms.EmailField(required=True)
#     contact_number = forms.CharField(max_length=12,required=True)
#     roll_number =forms.CharField(required=True)
#     first_name = forms.CharField(required=True) 
#     last_name = forms.CharField(required=True)
#     department = forms.CharField(required=True)
#     year_of_graduation = forms.IntegerField(required=True,min_value=1951,max_value=2021)

#     class Meta():
#         model = Alumni
#         fields = ['email','contact_number','first_name','last_name','department','year_of_graduation']

#     @transaction.atomic  #if an exception occurs changes are not saved
#     def save(self):
#         return self.cleaned_data.get('contact_number')

# class CompanySignUpForm(UserCreationForm):
#     email = forms.EmailField(label="Email (Email will be your username for Logging in the portal)")
#     contact_number = forms.CharField(max_length=12)
#     # first_name = forms.CharField(required=True) 
#     # last_name = forms.CharField(required=True)
#     profile = forms.CharField(required=True)
#     company_name =forms.CharField(required=True)
#     address = forms.CharField(required=True)
#     profile = forms.ChoiceField(choices=Profiles_choices)
#     # overview = forms.Textarea(attrs={"cols": "35", "rows": "10"})
#     # work_environ = forms.Textarea(attrs={"cols": "35", "rows": "10"})
#     # job_desc = forms.Textarea(attrs={"cols": "35", "rows": "10"})
#     # other_details = forms.Textarea(attrs={"cols": "35", "rows": "10"})
#     verify_doc = forms.FileField(label="Upload your Verification Documents(Single PDF)",required=False)

#     class Meta(UserCreationForm.Meta):
#         model = User
#         fields = ['email','contact_number','company_name','address','profile'] 
    
#     @transaction.atomic#if an exception occurs changes are not saved
#     def save(self):
#         user = super().save(commit=False)#before saving save the details
#         user.is_company = True
#         user.contact_number=self.cleaned_data.get('contact_number')
#         user.email = self.cleaned_data.get('email')
#         user.username = user.email
#         user.save()
#         company = Company.objects.create(user=user)#instance created
#         company.profile=self.cleaned_data.get('profile')
#         company.company_name=self.cleaned_data.get('company_name')
#         company.address=self.cleaned_data.get('address')
#         company.verify_doc = self.cleaned_data.get('verify_doc')
#         company.save()
#         return user


# class CompanydescForm(forms.ModelForm):
#     overview = forms.Textarea(attrs={"cols": "35", "rows": "10"})
#     work_environ = forms.Textarea(attrs={"cols": "35", "rows": "10"})
#     job_desc = forms.Textarea(attrs={"cols": "35", "rows": "10","required": False})
#     other_details = forms.Textarea(attrs={"cols": "35", "rows": "10"})

#     class Meta:
#         model = Company
#         fields = ['overview','work_environ','job_desc','other_details']


#     @transaction.atomic#if an exception occurs changes are not saved
#     def save(self):
#         return self.cleaned_data.get('overview'),self.cleaned_data.get('work_environ'),self.cleaned_data.get('job_desc'),self.cleaned_data.get('other_details')

# class CompanyEditForm(forms.ModelForm):
    # email = forms.EmailField(required=True)
    # contact_number = forms.CharField(max_length=12,required=True)
    # company_name =forms.CharField(required=True)
    # address = forms.CharField(required=True)
    # profile = forms.ChoiceField(choices=Profiles_choices)
    # overview = forms.Textarea(attrs={"cols": "35", "rows": "10"})
    # work_environ = forms.Textarea(attrs={"cols": "35", "rows": "10"})
    # job_desc = forms.Textarea(attrs={"cols": "35", "rows": "10"})
    # other_details = forms.Textarea(attrs={"cols": "35", "rows": "10"})

    # class Meta():
    #     model = Company
    #     fields = ['email','contact_number','company_name','address','profile','overview','work_environ','job_desc','other_details']

    # @transaction.atomic  #if an exception occurs changes are not saved
    # def save(self):
    #     return self.cleaned_data.get('contact_number'),self.cleaned_data.get('address'),self.cleaned_data.get('profile'),self.cleaned_data.get('overview'),self.cleaned_data.get('work_environ'),self.cleaned_data.get('job_desc'),self.cleaned_data.get('other_details')