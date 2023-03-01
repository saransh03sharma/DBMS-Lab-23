from django.contrib.auth import login, logout,authenticate
from django.shortcuts import redirect, render
from django.contrib import messages
from django.views.generic import CreateView
from django.http import HttpResponse
from django.contrib.auth.forms import AuthenticationForm
from .models import *
from .form import *
from django.core.mail import send_mail
from django.conf import settings
import datetime
import pytz
from django.conf import settings
from django.utils.timezone import make_aware

def register(request):
    return render(request, '../templates/register.html')

class patient_reg_help(CreateView):
    model = patient
    form_class = patient_register
    template_name = '../templates/edit_details.html'
    
    def get(self, request):
        return render(request,'../templates/edit_details.html',{'whereto':'patient_reg','form':patient_register})#display the form in the edit_details.html
    
    def form_valid(self,form):#form valid function
        if 'user' in self.request.session and 'type' in self.request.session:#if request is from an authenticated user 
            Email_ID, SSN, First_Name,Last_Name, Address, Insurance_ID, Phone, Age,Blood_Group, Status = form.save()#get data from form
            pa = patient(Email_ID=Email_ID,First_Name=First_Name,Last_Name=Last_Name,SSN = SSN, Address = Address, Age = Age, Insurance_ID = Insurance_ID,Blood_Group=Blood_Group,
                              Phone=Phone, Status = Status)
            pa.save()
        return redirect('/')
    
class doctor_register(CreateView):
    form_class = DoctorSignUpForm #student form specified
    template_name = '../templates/doctor_register.html' #template spcified

    def form_valid(self, form): #form valid check
        user = form.save() #save the form
        return redirect('/') #redirect to main index page
    
class front_desk_register(CreateView):
    form_class = FrontSignUpForm #student form specified
    template_name = '../templates/front_desk_register.html' #template spcified

    def form_valid(self, form): #form valid check
        user = form.save() #save the form
        return redirect('/') #redirect to main index page
    
class data_entry_register(CreateView):
    form_class = DataSignUpForm #student form specified
    template_name = '../templates/data_entry_register.html' #template spcified

    def form_valid(self, form): #form valid check
        user = form.save() #save the form
        return redirect('/') #redirect to main index page
  

def login_admin(request):
    print("hello")
    if request.method=='POST':#all requests withing the software are post and requests betwenn user and software is get
        print("h1h")
        username = request.POST['username']
        password = request.POST['password']
        
        try:
            
            user = db_admin.objects.get(username = username, password = password) 
            request.session['user'] = user.username
            request.session['type'] = "db_admin"
            return redirect('/')
            
        except Exception as e:
            print(e)
            return render(request, '../templates/login.html',#return to the template
                    context={'form':AuthenticationForm(), 'whereto':'admin_login'})
        
    elif request.method=='GET':
        if 'user' in request.session and 'type' in request.session:
            print(request.session['user'])
            return redirect('/')
    return render(request, '../templates/login.html',#return to the template
    context={'form':AuthenticationForm(), 'whereto':'admin_login'})

def login_doctor(request):
    if request.method=='POST':#all requests withing the software are post and requests betwenn user and software is get

        username = request.POST['username']
        password = request.POST['password']
        
        try:
            user = physician.objects.get(Email_ID = username)
            if check_password(password, user.password):
                request.session['user'] = user.Email_ID
                request.session['type'] = "doctor"
                return redirect('/')    
        except:
            return render(request, '../templates/login.html',#return to the template
                    context={'form':AuthenticationForm(),'whereto':'doctor_login'})
    
        
    elif request.method=='GET':
        if 'user' in request.session and 'type' in request.session:
            print(request.session['user'])
            return redirect('/')
    return render(request, '../templates/login.html',#return to the template
    context={'form':AuthenticationForm(),'whereto':'doctor_login'})

def login_fr(request):
    if request.method=='POST':#all requests withing the software are post and requests betwenn user and software is get

        username = request.POST['username']
        password = request.POST['password']
        
        
        try:
            user = front_desk.objects.get(Email_ID = username)
            print("hh")
            if check_password(password, user.password):
                print("jj")
                request.session['user'] = user.Email_ID
                request.session['type'] = "front_desk"
                return redirect('/')    
        except:
            return render(request, '../templates/login.html',#return to the template
                    context={'form':AuthenticationForm(),'whereto':'fr_login'})
    
        
    elif request.method=='GET':
        if 'user' in request.session and 'type' in request.session:
            print(request.session['user'])
            return redirect('/')
    return render(request, '../templates/login.html',#return to the template
    context={'form':AuthenticationForm(),'whereto':'fr_login'})

def login_de(request):
    if request.method=='POST':#all requests withing the software are post and requests betwenn user and software is get

        username = request.POST['username']
        password = request.POST['password']
        
        
        try:
            user = data_entry.objects.get(Email_ID = username)
            if check_password(password, user.password):
                request.session['user'] = user.Email_ID
                request.session['type'] = "data_entry"
                return redirect('/')    
        except:
            return render(request, '../templates/login.html',#return to the template
                    context={'form':AuthenticationForm(),'whereto':'de_login'})
    
        
    elif request.method=='GET':
        if 'user' in request.session and 'type' in request.session:
            print(request.session['user'])
            return redirect('/')
    return render(request, '../templates/login.html',#return to the template
    context={'form':AuthenticationForm(),'whereto':'de_login'})

def logout_view(request):#logout request
    if 'user' in request.session and 'type' in request.session:
        del request.session['user']
        del request.session['type']
        logout(request)#logout 
    return redirect('/')#redirect to accounts/index that will call index function


# def get_cv(request):
#     if (request.method == 'GET'):
#         if(request.user.is_authenticated):#if user is authenticated that is request made after logging in
#             user = User.objects.get(username = (request.user))
#             student = Student.objects.get(user = user)#get the user first corresponding to that credential and then extract the student using this user
#             cvs = {#extract cv
#                 'sd' : student.CV_SD,
#                 'da' : student.CV_DA
#             }
#             """ print(student.CV_SD)
#             print(type(student.CV_SD))
#              """
#             notif = Notification.objects.all().order_by('-Date')
#             return render(request,'../templates/index.html',{'cvs':cvs,'student':student,'notif':notif})#pass the extracted cv and other parameters required to index.html
#         return redirect('/')

# def str_to_lis(s):#to convert a string with comma seperated emails to a list of emails
#     if(s==""):
#         a=[]
#         return a
#     return s.split(',')

# def lis_to_str(b):#convert a list of emails to a string of comma sepearted string
#     if len(b)==0:
#         return ""
#     a = str(b[0])
#     for x in b:
#         if(x==b[0]):
#             continue
#         else:
#             a+= ',' + x
#     return a

# def remove_alum_pend(alumni_username,student_username):#to remove alumni from pending list
    
#     alumni = User.objects.get(username=alumni_username)
#     alumni = Alumni.objects.get(user = alumni)#first extract alumni using user with alumni_username
#     student = User.objects.get(username=student_username)
#     student = Student.objects.get(user = student)#extract student using user with student_username
    
#     alu= str_to_lis(alumni.list_of_stud_pend)#extract list of students pending from alumni database
#     stu= str_to_lis(student.list_of_alum_pend)#extract list of alumni pending from student database

#     for x in alu:
#         if(x == student.user.username):
#             alu.remove(x)#remove from pending list
            
#     for x in stu:
#         if(x == alumni.user.username):
#             stu.remove(x)#remove from student list
            
#     alumni.list_of_stud_pend = lis_to_str(alu)#update the list
#     student.list_of_alum_pend = lis_to_str(stu)#update the list

#     alumni.save()
#     student.save()
#     return student.list_of_alum_pend, alumni.list_of_stud_pend

# def add_alum_pend(alumni_user_id,student_user_id):#add to pending list
#     alumni_user = User.objects.get(username = (alumni_user_id))
#     student_user = User.objects.get(username = (student_user_id))
#     alumni = Alumni.objects.get(user=alumni_user)#extract alumni object
#     student = Student.objects.get(user=student_user)#extract student object
    
#     alu= str_to_lis(alumni.list_of_stud_pend)
#     stu= str_to_lis(student.list_of_alum_pend)
    
#     alu_username = alumni_user.username
#     stu_username = student_user.username
#     if stu_username not in alu:
#         alu.append(stu_username) #append the username to the list
#     if alu_username not in stu:
#         stu.append(alu_username)
#     alumni.list_of_stud_pend = lis_to_str(alu)#convert the list to comma sepearted string 
#     student.list_of_alum_pend = lis_to_str(stu)
    
#     alumni.save()
#     student.save()
#     return

# def add_alum(alumni_user_id,student_user_id):
#     alumni_user = User.objects.get(username = (alumni_user_id))
#     student_user = User.objects.get(username = (student_user_id))
#     alumni = Alumni.objects.get(user=alumni_user)
#     student = Student.objects.get(user=student_user)
#     alu= str_to_lis(alumni.list_of_stud)#list of student is students with which the alumni has already talked
#     stu= str_to_lis(student.list_of_alum)
#     alu_username = alumni_user.username
#     stu_username = student_user.username
#     if stu_username not in alu:
#         alu.append(stu_username)
#     if alu_username not in stu:
#         stu.append(alu_username)
#     # print(alu,stu)
    
#     alumni.list_of_stud = lis_to_str(alu)
#     student.list_of_alum = lis_to_str(stu)
    
#     alumni.save()
#     student.save()
#     return

# def list_of_students(request):
#     if(request.method == 'GET'):
#         if(request.user.is_authenticated):
#             if(request.user.is_company):
#                 user = User.objects.get(username = (request.user))
#                 comp = Company.objects.get(user = user) #extract company of user = request.user
#                 lis_of_studs = str_to_lis(comp.list_of_students)#list of students
#                 list_of_short_studs = str_to_lis(comp.list_of_short_students)#list of shortlisted students
#                 list_of_studs = []
#                 stud_detail = {}
#                 for stud in lis_of_studs:#iterate over list
#                     suser = User.objects.get(username = stud) #get student
#                     stud_detail['first_name']= suser.first_name
#                     stud_detail['last_name']= suser.last_name
#                     stud_detail['id']= suser.username
#                     list_of_studs.append(stud_detail.copy())#add the relevant details to the list
#                 return render(request,'../templates/list_of_students.html',{'whereto':'list_of_students','list_of_studs':list_of_studs,'list_of_short_studs':list_of_short_studs})
#         return redirect('/')
#     if(request.method == 'POST'):
#         if(request.user.is_authenticated):
#             if(request.user.is_company):#if request made by student
#                 user = User.objects.get(username = (request.user))
#                 comp = Company.objects.get(user = user)
#                 lis_of_studs = str_to_lis(comp.list_of_students)
#                 list_of_short_studs = str_to_lis(comp.list_of_short_students)
#                 list_of_studs = []
#                 stud_detail = {}
#                 for stud in lis_of_studs:
#                     suser = User.objects.get(username = stud)
#                     stud_detail['first_name']= suser.first_name
#                     stud_detail['last_name']= suser.last_name
#                     stud_detail['id']= suser.username
#                     list_of_studs.append(stud_detail.copy())
#                 stud_username = request.POST.get("stud_id")
#                 if (stud_username!=None):
#                     stud_user = User.objects.get(username = (stud_username))
#                     student = Student.objects.get(user = stud_user)
#                     if(comp.profile == "SD"):
#                         if(student.CV_SD):
#                             return redirect(student.CV_SD.url)
#                         else:
#                             flag=student.user.username
#                             return render(request,'../templates/list_of_students.html',{'whereto':'list_of_students','list_of_studs':list_of_studs,'list_of_short_studs':list_of_short_studs,'flag':flag})
#                     elif(comp.profile == "DA"):
#                         if(student.CV_DA):
#                             return redirect(student.CV_DA.url)
#                         else:
#                             flag=student.user.username
#                             return render(request,'../templates/list_of_students.html',{'whereto':'list_of_students','list_of_studs':list_of_studs,'list_of_short_studs':list_of_short_studs,'flag':flag})
        
#                 else:
#                     stud_username = request.POST.get("stud_shortlist_id") #to send email to shortlisted students
#                     if (stud_username!=None):
#                         stud_user = User.objects.get(username = (stud_username))
#                         student = Student.objects.get(user = stud_user)
#                         list_of_short_studs = str_to_lis(comp.list_of_short_students)
#                         if stud_username not in list_of_short_studs:
#                             list_of_short_studs.append(stud_username)
#                             comp.list_of_short_students = lis_to_str(list_of_short_studs)
#                             comp.save()
#                             e_mess_stud = "Congrats <b>" + str(stud_user.first_name) + ", "+str(student.roll_number) + "</b> you have been shortlisted by a company named <b>"+str(comp.company_name)+"</b><br>Further information about the further rounds of placement process will be conveyed by the Company Administrator via Email.<br><br>For any queries, you can contact the Institute Admin or you can also contact the Company Admin at <b>"+str(user.email)+"</b><br><br>Best,<br>OPIGS Team<br><br>"
#                             send_mail(
#                                 "SHORTLISTED BY A COMPANY", #subject
#                                 "", #message
#                                 "opigs.iitkgp@gmail.com", #from_email
#                                 [stud_user.email], #to_email_list
#                                 fail_silently=True,
#                                 html_message= e_mess_stud
#                             )
#                 list_of_short_studs = str_to_lis(comp.list_of_short_students)
#                 return render(request,'../templates/list_of_students.html',{'whereto':'list_of_students','list_of_studs':list_of_studs,'list_of_short_studs':list_of_short_studs})
#         return redirect('/')

# def request_feedback(request):#funct to request feedback from alumni
#     if(request.method == 'GET'):
#         if(request.user.is_authenticated):
#             alum_users = User.objects.filter(is_alumni = True)#extract all alumni
#             return render(request,'../templates/request_feedback.html',{'whereto':'request_feedback','alum_users':alum_users})#return the list of alums in case of get request
#         return redirect('/')

#     if(request.method == 'POST'):#post request would be made once user clicks on button
#         if(request.user.is_authenticated):
#             alum_users = User.objects.filter(is_alumni = True)
#             alum_username = request.POST.get("alum_id")#get the id of the alum using the button clicked
#             add_alum_pend(alum_username,request.user)#add it to pending list
#             return render(request,'../templates/request_feedback.html',{'whereto':'request_feedback','alum_users':alum_users})
#         return redirect('/')

# def feedback(request):#func to give feedback
#     if(request.method == 'GET'):
#         if(request.user.is_authenticated):
#             stud = User.objects.get(username = request.user)
#             alum=Alumni.objects.get(user=stud)
#             stud_list=str_to_lis(alum.list_of_stud_pend)
#             list_of_studs = []
#             stud_detail = {}
#             for stud in stud_list:
#                 suser = User.objects.get(username = stud)
#                 student = Student.objects.get(user = suser)
#                 stud_detail['first_name']= suser.first_name
#                 stud_detail['last_name']= suser.last_name
#                 stud_detail['id']= suser.username
#                 if(student.CV_SD):
#                     stud_detail['sd']= student.CV_SD.url
#                 if(student.CV_DA):
#                     stud_detail['da']= student.CV_DA.url
#                 list_of_studs.append(stud_detail.copy())
#                 stud_detail.clear()
#             return render(request,'../templates/feedback.html',{'whereto':'feedback','stud_list':list_of_studs})#display list of students and related information in case of get request
#         return redirect('/')

#     if(request.method == 'POST'):
#         if(request.user.is_authenticated):
#             stud_username = request.POST.get("alum_id")#get the student corresponding to whom the button is clicked
#             message = request.POST.get("feed")
#             chat=Chat.objects.create()#create a chat to store this feedback
#             chat.stud_username = stud_username
#             chat.alum_username = request.user.username
#             chat.Sender = 'A'#chat of sender alumni
#             chat.chat = message
#             chat.save()
#             stud,alum = remove_alum_pend(request.user,stud_username)
#             add_alum(request.user.username,stud_username)
#             stud = User.objects.get(username = request.user)
#             alum=Alumni.objects.get(user=stud)
#             stud_list=str_to_lis(alum.list_of_stud_pend)
#             list_of_studs = []
#             stud_detail = {}
#             for stud in stud_list:
#                 suser = User.objects.get(username = stud)
#                 student = Student.objects.get(user = suser)
#                 stud_detail['first_name']= suser.first_name
#                 stud_detail['last_name']= suser.last_name
#                 stud_detail['id']= suser.username
#                 if(student.CV_SD):
#                     stud_detail['sd']= student.CV_SD.url
#                 if(student.CV_DA):
#                     stud_detail['da']= student.CV_DA.url
#                 list_of_studs.append(stud_detail.copy())
#                 stud_detail.clear()
#             return render(request,'../templates/feedback.html',{'whereto':'feedback','stud_list':list_of_studs})
#         return redirect('/')

# def get_stud_cv(request):#to get cv
#      if(request.method == 'POST'):
#         if(request.user.is_authenticated):
#             stud_username = request.POST.get("alum_id")#get the username of student using the button
#             stud_user = User.objects.get(username = (stud_username))
#             student = Student.objects.get(user = stud_user)
#             if(student.CV_SD):
#                 return redirect(student.CV_SD.url)
#             else:
#                 flag=student.user.username

# def chats(request):
#      if(request.user.is_student):
#         if(request.method == 'GET'):#if student wants to chat
#             user=User.objects.get(username = request.user.username)
#             student=Student.objects.get(user = user)
#             clist=str_to_lis(student.list_of_alum)
#             list_of_alums = []
#             stud_detail = {}
#             for stud in clist:
#                 suser = User.objects.get(username = stud)
#                 student = Alumni.objects.get(user = suser)
#                 stud_detail['first_name']= suser.first_name
#                 stud_detail['last_name']= suser.last_name
#                 stud_detail['id']= suser.username
#                 list_of_alums.append(stud_detail.copy())
#                 stud_detail.clear()
#             # print(list_of_alums)
#             return render(request,"../templates/list_of_chats.html",{'whereto':'stud_chat','list_of_alums':list_of_alums})#show the list of alums he has talked with
#      if(request.user.is_alumni):
#         if(request.method == 'GET'):
#             user=User.objects.get(username = request.user.username)
#             student=Alumni.objects.get(user = user)
#             clist=str_to_lis(student.list_of_stud)
#             list_of_alums = []
#             stud_detail = {}
#             for stud in clist:
#                 suser = User.objects.get(username = stud)
#                 student = Student.objects.get(user = suser)
#                 stud_detail['first_name']= suser.first_name
#                 stud_detail['last_name']= suser.last_name
#                 stud_detail['id']= suser.username
#                 list_of_alums.append(stud_detail.copy())
#                 stud_detail.clear()
#             # print(list_of_alums)
#             return render(request,"../templates/list_of_chats.html",{'whereto':'stud_chat','list_of_alums':list_of_alums})#show the list of students he has talked with
        
# def stud_chat(request):
#     if(request.user.is_student):
#         if(request.method == 'GET'):#in case of getv request
#             if(request.user.is_authenticated):
#                 user=User.objects.get(username = request.user.username)
#                 student=Student.objects.get(user = user)
#                 alum_username = request.GET.get("alum_id")
#                 clist=str_to_lis(student.list_of_alum)
#                 plist=[]
#                 for x in clist:
#                     if x==alum_username:
#                         plist.append(x)
#                 chat=Chat.objects.filter(stud_username = request.user.username)
#             return render(request,"../templates/stud_chat.html",{'whereto':'stud_chat','chat':chat,'alum':plist})
        
#         if(request.method == 'POST'):
#             if(request.user.is_authenticated):
#                 alum_username = request.POST.get("alum_id")
                
#                 message = request.POST.get("feed")
                
#                 chat=Chat.objects.create()#create a new chat object
#                 chat.stud_username = request.user.username
#                 chat.alum_username = alum_username
#                 chat.Sender = 'S'
#                 chat.chat = message
#                 if len(message) != 0:
#                     chat.save()
#                 user=User.objects.get(username = request.user.username)
#                 student=Student.objects.get(user = user)
#                 clist=str_to_lis(student.list_of_alum)
#                 plist=[]
#                 for x in clist:
#                     if x==alum_username:
#                         plist.append(x)
#                 chat= Chat.objects.filter(stud_username = request.user.username)
#                 return render(request,'../templates/stud_chat.html',{'whereto':'stud_chat','chat':chat,'alum':plist})
            
#     if(request.user.is_alumni):#if laumni wants to contact
#             if(request.method == 'GET'):
#                 if(request.user.is_authenticated):
#                     user=User.objects.get(username = request.user.username)
#                     alumni=Alumni.objects.get(user = user)
#                     stud_username = request.GET.get("alum_id")
#                     clist=str_to_lis(alumni.list_of_stud)
#                     plist=[]
#                     for x in clist:
#                         if x==stud_username:
#                             plist.append(x)
#                     chat=Chat.objects.filter(alum_username = request.user.username)
#                 return render(request,"../templates/stud_chat.html",{'whereto':'stud_chat','chat':chat,'alum':plist})
            
#             if(request.method == 'POST'):
#                 if(request.user.is_authenticated):
#                     stud_username = request.POST.get("alum_id")
                    
#                     message = request.POST.get("feed")
                    
#                     chat=Chat.objects.create()
#                     chat.stud_username = stud_username
#                     chat.alum_username = request.user.username
#                     chat.Sender = 'A'
#                     chat.chat = message
#                     if len(message) != 0:
#                         chat.save()
#                     user=User.objects.get(username = request.user.username)
#                     alumni=Alumni.objects.get(user = user)
#                     clist=str_to_lis(alumni.list_of_stud)
#                     plist=[]
#                     for x in clist:
#                         if x==stud_username:
#                             plist.append(x)
#                     chat= Chat.objects.filter(alum_username = request.user.username)
#                     return render(request,'../templates/stud_chat.html',{'whereto':'stud_chat','chat':chat,'alum':plist})

# def add_stu_to_comp(company_user_id,student_user_id):
#     company_user = User.objects.get(username = (company_user_id))
#     student_user = User.objects.get(username = (student_user_id))
#     company = Company.objects.get(user=company_user)
#     student = Student.objects.get(user=student_user)
#     comp= str_to_lis(company.list_of_students)
#     stu= str_to_lis(student.list_of_comp)
#     comp_username = company_user.username
#     stu_username = student_user.username
#     if stu_username not in comp:
#         comp.append(stu_username)
#     if comp_username not in stu:
#         stu.append(comp_username)
#     company.list_of_students = lis_to_str(comp)
#     student.list_of_comp = lis_to_str(stu)
#     company.save()
#     student.save()
#     return

# def apply_company(request):
#     if(request.method == 'GET'):
#         if(request.user.is_authenticated):
#             user = User.objects.get(username = (request.user))
#             student = Student.objects.get(user = user)
#             # comp_users = User.objects.filter(is_company = True)
#             if (student.SDprofile  and student.DAprofile):
#                 # print("SD and DA")
#                 comps = Company.objects.filter(user__is_active=True)
#             elif(student.SDprofile):
#                 # print("SD")
#                 comps = Company.objects.filter(user__is_active=True , profile = "SD")
            
#             elif(student.DAprofile):
#                 # print("DA")
#                 comps = Company.objects.filter(user__is_active=True ,profile = "DA")
#             else:
#                 comps = Company.objects.none()

#             applied_comps = str_to_lis(student.list_of_comp)
#             return render(request,'../templates/apply_company.html',{'whereto':'apply_company','comps':comps,'applied_comps':applied_comps})
#         return redirect('/')

#     if(request.method == 'POST'):
#         if(request.user.is_authenticated):
#             comp_username = request.POST.get("comp_id")
#             add_stu_to_comp(comp_username,request.user)
#             user = User.objects.get(username = (request.user))
#             student = Student.objects.get(user = user)
#             # comp_users = User.objects.filter(is_company = True)
#             if (student.SDprofile  and student.DAprofile):
#                 # print("SD and DA")
#                 comps = Company.objects.filter(user__is_active=True)
#             elif(student.SDprofile):
#                 # print("SD")
#                 comps = Company.objects.filter(user__is_active=True ,profile = "SD")
            
#             elif(student.DAprofile):
#                 # print("DA")
#                 comps = Company.objects.filter(user__is_active=True ,profile = "DA")
#             else:
#                 comps = Company.objects.none()

#             applied_comps = str_to_lis(student.list_of_comp)
#             return render(request,'../templates/apply_company.html',{'whereto':'apply_company','comp_details_page':'company_details','comps':comps,'applied_comps':applied_comps})
#         return redirect('/')

# def company_details(request):
#     if(request.method == 'POST'):
#         if(request.user.is_authenticated):
            
#             comp_username = request.POST.get("comp_id")
#             user = User.objects.get(username = comp_username)
#             comp = Company.objects.get(user=user)
#             return render(request,'../templates/company_details.html',{'comp':comp})
#         return redirect('/')
#     else:
#         return redirect('/')


def handle_admit(request):
    if(request.method == 'GET'):
        if 'user' in request.session and 'type' in request.session:
            user = front_desk.objects.get(Email_ID = (request.session['user']))
            if user is not None:
                pat = patient.objects.all()
                return render(request,'../templates/admin_user.html',{'whereto':'handle_admit','pat':pat})
        return redirect('/')  
    elif request.method == 'POST':
        user = front_desk.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("comp_id")
            if a is not None:            
                try:
                    user = patient.objects.get(Email_ID = a)     
                    if user.Status==1:
                        return redirect("/admit_discharge")     
                    values = {
                            'First_Name':user.First_Name,
                            'Last_Name':user.Last_Name,
                        }
                    form = admit_pat(values)
                    form.fields['First_Name'].widget.attrs['readonly']  =True
                    form.fields['Last_Name'].widget.attrs['readonly']  =True
                    print(values)
                    return render(request,'../templates/admit_room.html',{'whereto':'patient_admit','form':form, 'Email_ID':user.Email_ID})#display the form in the edit_details.html
                except patient.DoesNotExist:
                    return redirect('/admit_discharge')
            a = request.POST.get("dis_id")
            if a is not None:
                try:
                    user = patient.objects.get(Email_ID = a)
                    if user.Status!=1:
                        return redirect("/admit_discharge")  
                    first_admit = admission.objects.filter(Patient_Email = a).order_by('-Start').first()
                    naive_datetime = datetime.datetime.now()
                    naive_datetime.tzinfo  # None

                    settings.TIME_ZONE  # 'UTC'
                    aware_datetime = make_aware(naive_datetime)
                    aware_datetime.tzinfo  # <UTC>
                    first_admit.End =  aware_datetime
                    x = (first_admit.End - first_admit.Start)
                    print(first_admit.End)
                    
                    user.Status=2
                    pat_room = room.objects.get(Room_ID = first_admit.Room_ID)
                    pat_room.Capacity += 1
                    first_admit.Total_Cost =  x.days * pat_room.Cost
                    print(first_admit.Total_Cost)
                    user.save()
                    first_admit.save()
                    pat_room.save()
                    user = patient.objects.get(Email_ID = a)
                    admit = admission.objects.filter(Patient_Email = a).order_by("-Admission_ID")
                    return render(request,'../templates/company_details.html',{'user':user, 'admit':admit})
                except Exception as e:
                    print(e)
                    return redirect("/admit_discharge")
            a = request.POST.get("info")
            if a is not None:
                try:
                    user = patient.objects.get(Email_ID = a)
                    admit = admission.objects.filter(Patient_Email = a).order_by("-Admission_ID")
                    print(user)
                    print(type(admit))
                    return render(request,'../templates/company_details.html',{'user':user, 'admit':admit})
                except Exception as e:
                    print(e)
                    return redirect("/admit_discharge")
            return redirect("/admit_discharge")
        return redirect("/")
 
class admit_patient(CreateView):
    model = undergoes
    form_class = admit_pat
    template_name = '../templates/admit_room.html'
    
    def get(self,request):
        return redirect("/admit_discharge")
    
    def form_valid(self,form):#form valid function
        if 'user' in self.request.session and 'type' in self.request.session:#if request is from an authenticated user 
            Email_ID = self.request.POST.get("my_variable")
            user = front_desk.objects.get(Email_ID = (self.request.session['user']))
            if user is not None:
                user = patient.objects.get(Email_ID = Email_ID)
                if user is not None:
                    First_Name,Last_Name, Room_ID, Start, PCP_Email = form.save()#get data from form
                    doct = physician.objects.get(Email_ID = PCP_Email)
                    if(Room_ID!="-1" and doct is not None):
                        # print(First_Name,Last_Name, Room_ID, Start)
                        pa = admission(Patient_Email =Email_ID, Room_ID = Room_ID, Start=Start, End=Start, PCP_Email = PCP_Email)

                        pat = patient.objects.get(Email_ID =Email_ID)
                        pat.Status = 1
                        pat_room = room.objects.get(Room_ID = Room_ID)
                        pat_room.Capacity -= 1 
                        pat_room.save()
                        pa.save()
                        pat.save()
        return redirect('/')


def schedule_appoint(request):
    if(request.method == 'GET'):
        if 'user' in request.session and 'type' in request.session:
            user = front_desk.objects.get(Email_ID = (request.session['user']))
            if user is not None:
                pat = patient.objects.all()
                print(pat)
        return redirect('/') 
    elif request.method == 'POST':
        user = front_desk.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("comp_id")
            if a is not None:
                # print("yes")
                user = patient.objects.get(Email_ID = a)
                values = {
                        'First_Name':user.First_Name,
                        'Last_Name':user.Last_Name,
                    }
                form = admit_pat(values)
                form.fields['First_Name'].widget.attrs['readonly']  =True
                form.fields['Last_Name'].widget.attrs['readonly']  =True
                print(values)
                return render(request,'../templates/admit_room.html',{'whereto':'patient_admit','form':form, 'Email_ID':user.Email_ID})
        return redirect('/')

def index(request): # to return homepage depending upon the logged in user
    if(request.method == 'POST'):
        print("hi")
        if 'user' in request.session and 'type' in request.session:
            try:
                print("hello")
                user = db_admin.objects.get(username = (request.session['user']))
                id = request.POST.get("front_id")
                if id is not None:    
                    front = front_desk.objects.get(Email_ID = id)
                    front.delete()
                id = request.POST.get("data_id")
                if id is not None:    
                    front = data_entry.objects.get(Email_ID = id)
                    front.delete()
                id = request.POST.get("doct_id")
                if id is not None:    
                    front = physician.objects.get(Email_ID = id)
                    front.delete()
                    
                front = front_desk.objects.all()
                print(front)
                data = data_entry.objects.all()
                print(data)
                doct = physician.objects.all()
                print(doct)
                return render(request, 'index.html', {'user': user, 'type':type, 'status':1,'whereto':'index','fronts':front,'datas':data,'docts':doct})
                
            except Exception as e:
                print(e)
                return redirect('/')  
    if(request.method == 'GET'):
        user_id = request.session.get('user')
        type = request.session.get('type')

        if type == 'doctor':
            try:
                user = physician.objects.get(Email_ID = user_id)
                return render(request, 'index.html', {'user': user, 'type':type, 'status':1})

            except physician.DoesNotExist:
                return render(request, 'index.html', {'user': {}, 'type':"none"})

        elif type == 'front_desk':
            try:
                user = front_desk.objects.get(Email_ID = user_id)
                return render(request, 'index.html', {'user': user, 'type':type, 'status':1})

            except front_desk.DoesNotExist:
                return render(request, 'index.html', {'user': {}, 'type':"none"})

        elif type == 'db_admin':
            try:
                user = db_admin.objects.get(username = user_id)
                front = front_desk.objects.all()
                print(front)
                data = data_entry.objects.all()
                print(data)
                doct = physician.objects.all()
                print(doct)
                return render(request, 'index.html', {'user': user, 'type':type, 'status':1,'whereto':'index','fronts':front,'datas':data,'docts':doct})
                

            except :
                return render(request, 'index.html', {'user': {}, 'type':"none"})

        elif type == 'data_entry':
            try:
                user = data_entry.objects.get(Email_ID = user_id)
                
                return render(request, 'index.html', {'user': user, 'type':type, 'status':1})

            except data_entry.DoesNotExist:
                return render(request, 'index.html', {'user': {}, 'type':"none"})

        else:
            return render(request, 'index.html')

def record_treatment(request):
    pass

def doctor_pat_record(request):

    # if (request.method == 'GET'):
    #     if 'user' in request.session and 'type' in request.session:
    #         try:
    #             user = physician.objects.get(Email_ID = (request.session['user']))
    #             return render(request, '../templates/doctor_pat_record.html', {'user': user, 'type':type, 'status':1})
    #         except Exception as e:
    #             print(e)
    #             return redirect('/')
    
    if (request.method == 'GET'):
        # return render(request, '../templates/doctor_pat_record.html', {'user': user, 'type':type, 'status':1})
        if 'user' in request.session and 'type' in request.session:
            try:
                user = physician.objects.get(Email_ID = (request.session['user']))
                print(user.Email_ID)
                # get details of patients' who have had an appointment with the doctor
                doctor_apts = appointment.objects.filter(Physician_Email = user.Email_ID)
                print(doctor_apts)
                patients = []  # list of patients
                for apt in doctor_apts:
                    pat = patient.objects.get(Email_ID = apt.Patient_Email)
                    patients.append(pat)
                print(patients)

                return render(request, '../templates/doctor_pat_record.html', {'user': user, 'type':type, 'status':1, 'patients':patients})
            except Exception as e:
                print(e)
                return redirect('/')

        


    


