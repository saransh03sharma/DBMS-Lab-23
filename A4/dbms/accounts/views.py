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
            # print("hh")
            if check_password(password, user.password):
                # print("jj")
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

def handle_admit(request):
    if(request.method == 'GET'):
        if 'user' in request.session and 'type' in request.session:
            user = front_desk.objects.get(Email_ID = (request.session['user']))
            if user is not None:
                pat = patient.objects.all()
                # print(pat)
                # adm = admission.objects.all()
                # for x in adm:
                #     print(x.Patient_Email,x.Room_ID,x.Start,x.End,x.PCP_Email,x.Total_Cost)
                return render(request,'../templates/admin_user.html',{'whereto':'handle_admit','pat':pat,'user':user})
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

def scheduler(request):
    if(request.method == 'POST'):
        user = front_desk.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("checker")
            if a is not None:
                pat = patient.objects.get(Email_ID = a)
                doc = request.POST.get("Physician_Email")
                date = request.POST.get("Start")
                fee = request.POST.get("Appointment_Fee")
                emergency = request.POST.get("Emergency")
                values = {
                    'Physician_Email': doc,
                    'Start': date,
                    'Appointment_Fee': fee,
                    'Emergency': emergency,
                }
                print(values)
                date = datetime.datetime.strptime(date, '%Y-%m-%d')
                date = make_aware(date)
                # print(date)
                appoints = []
                for i in range(10,20,1):
                    appoint = appointment.objects.filter(Physician_Email = doc, Start = (date+datetime.timedelta(hours=i)))
                    treat = undergoes.objects.filter(Physician_Email=doc, Date = (date+datetime.timedelta(hours=i)))
                    # print(date+datetime.timedelta(hours=i))
                    # print("hell")
                    if (emergency or ((appoint is None or len(appoint) == 0) and (treat is None or len(treat) == 0))):
                        time = str("{0:02d}:00 - {1:02d}:00".format(i, i+1))
                        # print(time)
                        appoints.append({
                            'id' : i,
                            'time' : time
                        })
                # print(len(appoints))
                form = schedule_app(values)
                return render(request,'../templates/scheduler.html',{'whereto':'scheduler', 'form':form, 'pat':pat,'user':user, 'slots':appoints, 'vals':values})
            a = request.POST.get("slot_id")
            if a is not None:
                a = int(a)
                pat = request.POST.get("Patient_Email")
                doc = request.POST.get("Physician_Email")
                date = request.POST.get("Start")
                fee = request.POST.get("Appointment_Fee")
                print(pat,doc,date,fee,a)
                date = datetime.datetime.strptime(date, '%Y-%m-%d')
                aware_date = make_aware(date)
                aware_date = aware_date + datetime.timedelta(hours=a)
                check_appoint = appointment.objects.filter(Physician_Email = doc, Start = aware_date)
                if check_appoint is not None and len(check_appoint) > 0:
                    print("Appointment already exists!")
                    print("Deleting the appointment")
                    check_appoint.delete()
                appoint = appointment(Patient_Email = pat, Physician_Email = doc, Start = (date+datetime.timedelta(hours=a)),Appointment_Fee = fee)
                appoint.save()
            return redirect('/schedule_appointment')
    return redirect('/')

def schedule_appoint(request):
    if(request.method == 'GET'):
        if 'user' in request.session and 'type' in request.session:
            user = front_desk.objects.get(Email_ID = (request.session['user']))
            if user is not None:
                pat = patient.objects.all()
                # print(pat)
                return render(request,'../templates/schedule_appoint.html',{'whereto':'schedule_appoint','pat':pat,'user':user})
        return redirect('/') 
    elif request.method == 'POST':
        user = front_desk.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("comp_id")
            if a is not None:
                pat = patient.objects.get(Email_ID = a)
                # print(pat)
                # form = schedule_appoint()
                if pat is not None:
                    return render(request,'../templates/scheduler.html',{'whereto':'scheduler','form':schedule_app,'user':user,'pat':pat})
        return redirect('/')

def index(request): # to return homepage depending upon the logged in user
    if(request.method == 'POST'):
        # print("hi")
        if 'user' in request.session and 'type' in request.session:
            try:
                # print("hello")
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
                rooms = room.objects.all()
                return render(request, 'index.html', {'user': user, 'type':type, 'status':1, 'rooms':rooms})

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
            # try:
            user = physician.objects.get(Email_ID = (request.session['user']))
            # print(user.Email_ID)
            if user is not None:
            # get details of patients' who have had an appointment with the doctor
                doctor_apts = appointment.objects.filter(Physician_Email = user.Email_ID)
                # print(doctor_apts)
                doctor_appoints = set()
                for apt in doctor_apts:
                    doctor_appoints.add(apt.Patient_Email)
                patients = []
                for apt in doctor_appoints: 
                    pat = patient.objects.get(Email_ID = apt)
                    patients.append(pat)

                return render(request, '../templates/doctor_pat_record.html', {'user': user, 'whereto': 'doctor_pat_record', 'patients':patients})
            # except Exception as e:
            #     print(e)
                # return redirect('/')
    if (request.method == 'POST'):
        if 'user' in request.session and 'type' in request.session:
            # try:
            user = physician.objects.get(Email_ID = (request.session['user']))
            if user is not None:
                a = request.POST.get('comp_id')
                if a is not None:
                    pat = patient.objects.get(Email_ID = a)
                    values = {
                        'First_Name': pat.First_Name,
                        'Last_Name': pat.Last_Name,
                        'Age': pat.Age,
                        'Blood_Group': pat.Blood_Group
                    }
                    form = prescribe_form(values)
                    form.fields['First_Name'].widget.attrs['readonly']  =True
                    form.fields['Last_Name'].widget.attrs['readonly']  =True
                    form.fields['Age'].widget.attrs['readonly']  =True
                    form.fields['Blood_Group'].widget.attrs['readonly']  =True
                    return render(request, '../templates/doctor_prescribes.html', {'user': user, 'whereto': 'prescribe_medic', 'form': form, 'pat':pat})
                
                b = request.POST.get('pat_email_health')
                if b is not None:
                    pat = patient.objects.get(Email_ID = b)
                    # filter health records of the patient based on the email id using the admission table'
                    pat_admits = admission.objects.filter(Patient_Email = pat.Email_ID)
                    print("Hello")
                    print(pat_admits)
                    print("Hello")

                    # get the health records of the patient for each admission
                    pat_health = []
                    for admit in pat_admits:
                        health = health_record.objects.filter(Admission_ID = admit.Admission_ID)
                        for h in health:
                            pat_health.append(h)

                    

                    print(pat_health)
                
        return redirect('/')
        
class doctor_prescribe(CreateView):
    model = prescribes
    form_class = prescribe_form
    template_name = '../templates/doctor_prescribes.html'

    def get(self, request):
        return redirect('doctor_pat_record')
        
    def form_valid(self, form):
        if 'user' in self.request.session and 'type' in self.request.session:
            user = physician.objects.get(Email_ID = (self.request.session['user']))
            print(user.Email_ID)
            First_Name,Last_Name,Age, Blood_Group,Prescribe_Date,Prescription = form.save()
            print(First_Name,Last_Name,Age, Blood_Group,Prescribe_Date,Prescription,self.request.POST.get('patient_id'))
            pres = prescribes(Physician_Email = user.Email_ID, Patient_Email = self.request.POST.get('patient_id'), Date=Prescribe_Date, Prescription=Prescription)
            pres.save()
            return redirect('doctor_pat_record')

        else:
            return redirect('/')


# def show_health_records(request):

#     if (request.method == 'GET'):
#         if 'user' in request.session and 'type' in request.session:

#             user = physician.objects.get(Email_ID = (request.session['user']))

#             if user is not None:


def show_upcoming_appts(request):
    if (request.method == 'GET'):
        if 'user' in request.session and 'type' in request.session:

            user = physician.objects.get(Email_ID = (request.session['user']))
            
            if user is not None:
                
                # get all doctor appointments starting from current day's appointment
                date = datetime.datetime.date(make_aware(datetime.datetime.now()))
                # print(date)
                # aware_datetime = datetime.datetime
                doctor_apts = appointment.objects.filter(Physician_Email = user.Email_ID, Start__gte = date)
                # doctor_apts = appointment.objects.filter(Physician_Email = user.Email_ID, Start__gte = make_aware(datetime.datetime.now()))
                print(doctor_apts)
                patients = set()
                for apt in doctor_apts:
                    pat = patient.objects.get(Email_ID = apt.Patient_Email)
                    patients.add(pat)

                print(patients)

                return render(request, '../templates/doctor_apts.html', {'user': user, 'whereto': 'show_upcoming_appts', 'appointments': doctor_apts, 'patients': patients})

    
def patient_data_entry(request):
    if(request.method == 'GET'):
        if 'user' in request.session and 'type' in request.session:
            user = data_entry.objects.get(Email_ID = (request.session['user']))
            if user is not None:
                pat = patient.objects.all()
                return render(request,'../templates/pat_list.html',{'whereto':'patient_data_entry','pat':pat,'user':user})
        
        return redirect('/')  
    elif request.method == 'POST':
        user = data_entry.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("completion")
            print(a)
            if a is not None:            
                try:
                    user = patient.objects.get(Email_ID = a)          
                    values = {
                            'First_Name':user.First_Name,
                            'Last_Name':user.Last_Name,
                        }
                    form = patient_test(values)
                    form.fields['First_Name'].widget.attrs['readonly']  =True
                    form.fields['Last_Name'].widget.attrs['readonly']  =True
                    print(values)
                    return render(request,'../templates/admit_room.html',{'whereto':'patient_test','form':form, 'Email_ID':user.Email_ID})#display the form in the edit_details.html
                except patient.DoesNotExist:
                    return redirect('/patient_test')
            a = request.POST.get("schedule_test")
            if a is not None:
                pat = patient.objects.get(Email_ID = a)
                if pat is not None:
                    return render(request,'../templates/scheduler.html',{'whereto':'scheduler_test','pat':pat,'user':user,'form':schedule_test})                
            a = request.POST.get("schedule_treatment")
            if a is not None:
                pat = patient.objects.get(Email_ID = a)
                if pat is not None:
                    return render(request,'../templates/scheduler.html',{'whereto':'scheduler_treatment','pat':pat,'user':user,'form':schedule_treatment})                
            a = request.POST.get("prescribe")
            if a is not None:            
                try:
                    user = patient.objects.get(Email_ID = a)          
                    values = {
                            'First_Name':user.First_Name,
                            'Last_Name':user.Last_Name,
                        }
                    form = admit_pat(values)
                    form.fields['First_Name'].widget.attrs['readonly']  =True
                    form.fields['Last_Name'].widget.attrs['readonly']  =True
                    print(values)
                    return render(request,'../templates/admit_room.html',{'whereto':'patient_operation','form':form, 'Email_ID':user.Email_ID})#display the form in the edit_details.html
                except patient.DoesNotExist:
                    return redirect('/patient_test')
        return redirect("/")
 
class test_patient(CreateView):
    model = tested
    form_class = patient_test
    template_name = '../templates/edit_details.html'
    
    def get(self, request):
        return redirect("/patient_test")
    
    def form_valid(self,form):#form valid function
        if 'user' in self.request.session and 'type' in self.request.session:#if request is from an authenticated user 
            
            print("hi")
        return redirect('/')


def scheduler_test(request):
    if(request.method == 'POST'):
        user = data_entry.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("checker")
            if a is not None:
                pat = patient.objects.get(Email_ID = a)
                doc = request.POST.get("Physician_Email")
                date = request.POST.get("Start")
                test_id = int(request.POST.get("Test_ID"))
                if test_id<0:
                    return redirect('/patient_data_entry')
                values = {
                    'Physician_Email':doc,
                    'Start':date,
                    'Test_ID':test_id,
                }
                date = datetime.datetime.strptime(date, '%Y-%m-%d')
                date = make_aware(date)
                # print(date)
                appoints = []
                for i in range(10,20,1):
                    appoint = tested.objects.filter(Test_ID = test_id, Date = (date+datetime.timedelta(hours=i)))
                    # print(date+datetime.timedelta(hours=i))
                    # print("hell")
                    if (appoint is None or len(appoint) < 5):
                        time = str("{0:02d}:00 - {1:02d}:00".format(i, i+1))
                        appoints.append({
                            'id' : i,
                            'time' : time
                        })
                print(len(appoints))
                form = schedule_test(values)
                return render(request,'../templates/scheduler.html',{'whereto':'scheduler_test', 'form':form, 'pat':pat,'user':user, 'slots':appoints, 'vals':values})
            a = request.POST.get("slot_id")
            if a is not None:
                a = int(a)
                pat = request.POST.get("Patient_Email")
                doc = request.POST.get("Physician_Email")
                date = request.POST.get("Start")
                test_id = int(request.POST.get("Test_ID"))
                date = datetime.datetime.strptime(date, '%Y-%m-%d')
                date = make_aware(date)
                date = date+datetime.timedelta(hours=a)
                test = tested(Patient_Email = pat, Date = date, Test_ID = test_id)
                test.save()
        return redirect("/patient_data_entry")
    return redirect("/")

def scheduler_treatment(request):
    if(request.method == 'POST'):
        user = data_entry.objects.get(Email_ID = (request.session['user']))
        if user is not None:
            a = request.POST.get("checker")
            if a is not None:
                pat = patient.objects.get(Email_ID = a)
                doc = request.POST.get("Physician_Email")
                date = request.POST.get("Start")
                treatment_id = int(request.POST.get("Treatment_ID"))
                if treatment_id < 0:
                    return redirect('/patient_data_entry')
                values = {
                    'Physician_Email':doc,
                    'Start':date,
                    'Treatment_ID':treatment_id,
                }
                date = datetime.datetime.strptime(date, '%Y-%m-%d')
                date = make_aware(date)
                # print(date)
                appoints = []
                for i in range(10,20,1):
                    treat = undergoes.objects.filter(Physician_Email=doc, Date = (date+datetime.timedelta(hours=i)))
                    appoint = appointment.objects.filter(Physician_Email=doc, Start = (date+datetime.timedelta(hours=i)))
                    # print(date+datetime.timedelta(hours=i))
                    # print("hell")
                    if ((appoint is None or len(appoint) == 0) and (treat is None or len(treat) == 0)):
                        time = str("{0:02d}:00 - {1:02d}:00".format(i, i+1))
                        appoints.append({
                            'id' : i,
                            'time' : time
                        })
                print(len(appoints))
                form = schedule_treatment(values)
                return render(request,'../templates/scheduler.html',{'whereto':'scheduler_treatment', 'form':form, 'pat':pat,'user':user, 'slots':appoints, 'vals':values})
            a = request.POST.get("slot_id")
            if a is not None:
                a = int(a)
                pat = request.POST.get("Patient_Email")
                doc = request.POST.get("Physician_Email")
                date = request.POST.get("Start")
                treatment_id = request.POST.get("Treatment_ID")
                date = datetime.datetime.strptime(date, '%Y-%m-%d')
                date = make_aware(date)
                date = date+datetime.timedelta(hours=a)
                treat = undergoes(Patient_Email = pat, Physician_Email = doc, Date = date, Treatment_ID = treatment_id)
                treat.save()
        return redirect("/patient_data_entry")
    return redirect("/")