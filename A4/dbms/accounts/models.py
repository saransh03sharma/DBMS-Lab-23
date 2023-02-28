from django.db import models

class db_admin(models.Model):
    
    username = models.CharField(max_length=255, primary_key=True)
    password = models.CharField(max_length=255)
    
    def __str__(self):
        return self.username

class front_desk(models.Model):
    
    FirstName = models.CharField(max_length=255)
    LastName = models.CharField(max_length=255)
    Email = models.EmailField(primary_key=True)
    EmployeeID = models.IntegerField()
    password = models.CharField(max_length=255)
    
    def __str__(self):
        return self.Email
    
class data_entry(models.Model):
    
    FirstName = models.CharField(max_length=255)
    LastName = models.CharField(max_length=255)
    Email = models.EmailField(primary_key=True)
    EmployeeID = models.IntegerField()
    password = models.CharField(max_length=255)
    
    def __str__(self):
        return self.Email

class physician(models.Model):
    
    Email_id = models.EmailField(primary_key=True)
    EmployeeID = models.IntegerField()
    FirstName = models.CharField(max_length = 255)
    LastName = models.CharField(max_length = 255)
    Position = models.CharField(max_length = 255)
    Department = models.CharField(max_length=255)
    password = models.CharField(max_length=255)
    
    class Meta:
        unique_together = (('Email_id','EmployeeID'),)
        
    def __str__(self):
        return self.FirstName+" "+self.LastName
    
class patient_test(models.Model):

    ID = models.AutoField(primary_key=True)
    Patient = models.EmailField()
    TestID = models.IntegerField()
    Date = models.DateTimeField()
    Test_result = models.TextField()
   
 
    def __str__(self):
        return str(self.ID)+" "+self.Patient

class tests(models.Model):

    TestID = models.AutoField(primary_key=True)
    TestName = models.CharField(max_length = 255)
    Cost = models.IntegerField()
   
 
    def __str__(self):
        return str(self.TestID) + " " + self.TestName

class treatment(models.Model):

    TreatmentID = models.AutoField(primary_key=True)
    TreatmentName = models.CharField(max_length = 255)
    Cost = models.IntegerField()
   
 
    def __str__(self):
        return str(self.TreatmentID) + " " + self.TreatmentName

class room(models.Model):

    Number = models.IntegerField()
    Type = models.CharField(max_length = 255)
    Room_name = models.CharField(max_length=255)
    Capacity = models.IntegerField()
    Cost = models.IntegerField()
   
   
    class Meta:
        unique_together = (('Number','Room_name'),)
 
    def __str__(self):
        return self.Type + " " + str(self.Number)
    
class patient(models.Model):

    Email_id = models.CharField(max_length=255, primary_key=True)
    SSN = models.IntegerField()
    FirstName = models.CharField(max_length = 255)
    LastName = models.CharField(max_length = 255)
    Address = models.CharField(max_length = 255)
    Phone = models.CharField(max_length = 255)
    InsuranceID = models.IntegerField()
    Age = models.IntegerField()
    BloodGroup = models.CharField(max_length=255)
    Status = models.IntegerField()
   
 
    def __str__(self):
        return self.Email_id
    
class undergoes(models.Model):

    UndergoesID = models.AutoField(primary_key=True)
    Patient = models.EmailField()
    TreatmentID = models.IntegerField()
    Date = models.DateTimeField()
    Physician = models.EmailField()
    
   
    class Meta:
        unique_together = (('Patient','TreatmentID', 'Physician', 'Date'),)
 
    def __str__(self):
        return str(self.UndergoesID) + " " + str(self.Patient) 
    
class admission(models.Model):
    Admissionid = models.AutoField(primary_key=True)
    Patient = models.EmailField()
    Room = models.IntegerField()
    Start = models.DateTimeField()
    End = models.DateTimeField()
    PCP_email = models.EmailField()
    Total_Cost = models.IntegerField()
    
 
    def __str__(self):
        return str(self.Admissionid) + " " + str(self.Patient)   

class prescribes(models.Model):

    PrescribeID = models.AutoField(primary_key=True)
    Physician = models.EmailField()
    Patient = models.EmailField()
    Date = models.DateTimeField()
    Prescription = models.TextField()
   
    class Meta:
        unique_together = (('Physician','Patient', 'Date'),)
 
    def __str__(self):
        return str(self.PrescribeID) + " " + str(self.Patient) 

class health_record(models.Model):

    RecordID = models.AutoField(primary_key=True)
    Patient = models.EmailField()
    Date = models.DateTimeField()
    Vitals = models.TextField()
    Remarks = models.TextField()
 
    def __str__(self):
        return str(self.RecordID) + " " + self.Patient
    
class appointment(models.Model):

    AppointmentID = models.AutoField(primary_key=True)
    Patient = models.EmailField()
    Physician = models.EmailField()
    Start = models.DateTimeField()
    AppointmentFee = models.IntegerField()
    
 
    def __str__(self):
        return str(self.AppointmentID)+" "+self.Patient
    

    



# class User(AbstractUser): #user model defined which would be inherited by other models
#     is_student=models.BooleanField(default=False) #to track if the user is student
#     is_company=models.BooleanField(default=False) #to track if the user is company
#     is_alumni=models.BooleanField(default=False) #to track if the user is alumni
#     username = models.CharField(max_length=50,unique=True, default=uuid.uuid1) #username
#     first_name = models.CharField(default = "demo",max_length=100) #first name
#     last_name = models.CharField(default = "demo",max_length=100) #last name
#     email = models.EmailField(unique=True) #email
#     contact_number = models.CharField(max_length=10) #contact number
#     REQUIRED_FIELDS = []#which fields are required fields
    
#     def __str__(self): #this specifies how the user should be refered to in database
#         return self.username 

# class Student(models.Model):
#     user = models.OneToOneField(User, on_delete = models.CASCADE) #class student is defined which is defined over user. one to one relation between the two is specified
#     department = models.CharField(max_length=60)
#     roll_number = models.CharField(max_length=9,unique=True)#roll number should be unique
#     list_of_comp = models.TextField(default="",verbose_name="List of Companies Applied",blank=True) #verbose name is alternate name that would appear in the database instead of original name
#     list_of_alum = models.TextField(default="",verbose_name="List of Alumni Talked",blank=True)
#     list_of_alum_pend = models.TextField(default="",verbose_name="List of Alumni request pending",blank=True)
#     SDprofile = models.BooleanField(default=True,verbose_name="SDE Profile Chosen") 
#     DAprofile = models.BooleanField(default=False,verbose_name="DA Profile Chosen")

#     #Add FileField CV
#     CV_SD = models.FileField(verbose_name="CV of Software Profile",blank=True,null=True,default=None,upload_to='accounts/resumes')#upload to would store the uploaded doc in the local computer
#     CV_DA = models.FileField(verbose_name="CV of Data Analytics Profile",blank=True,null=True,default=None,upload_to='accounts/resumes')
    
#     def __str__(self):
#         return self.user.first_name + " " + self.user.last_name

# class Alumni(models.Model):
#     user = models.OneToOneField(User, on_delete = models.CASCADE) #one to one relation is defined
#     roll_number = models.CharField(max_length=9,unique=True)
#     department = models.CharField(max_length=60)
#     year_of_graduation = models.IntegerField(default="2020")
#     list_of_stud = models.TextField(default="",verbose_name="List of Student talked",blank=True)
#     list_of_stud_pend = models.TextField(default="",verbose_name="List of Student request pending",blank=True)
    
#     def __str__(self):
#         return self.user.first_name + " " + self.user.last_name
    
# class Company(models.Model):
#     user = models.OneToOneField(User, on_delete = models.CASCADE)
#     company_name = models.CharField(max_length=70)
#     address = models.CharField(max_length=100)
#     profile = models.CharField(max_length=5,verbose_name="Select one Profile for which you want to recruit")
#     overview = models.TextField(default="",verbose_name="Company Overview(to display on Company Page)",blank=True)
#     work_environ = models.TextField(default="",verbose_name="Work Environment",blank=True)
#     job_desc = models.TextField(default="",verbose_name="Job Description",blank=True)
#     other_details = models.TextField(default="",verbose_name="Some Other details(if any)",blank=True)
#     verify_doc = models.FileField(verbose_name="Upload your Verification Documents(Single PDF)",blank=True,null=True,default=None,upload_to='accounts/company_req')#upload to specifies location wherein the doc will be uploaded
#     list_of_students = models.TextField(default="",verbose_name="List of Students applied",blank=True)
#     list_of_short_students = models.TextField(default="",verbose_name="List of Students Shortlisted",blank=True)
#     USERNAME_FIELD = 'email'
    
#     def __str__(self):
#         return self.company_name

# class Notification(models.Model):#to store notifications
#     Date=models.DateTimeField(auto_now_add=True)#auto timestamp added
#     Subject=models.CharField(max_length=40,default="Alert")#default subject added
#     Notification = models.TextField()
        
#     def __str__(self):
#         return self.Subject


# class Chat(models.Model):#to store chats
#     stud_username = models.EmailField() #username of student chatting
#     alum_username = models.EmailField() #username of alumni chatting
#     Sender = models.CharField(max_length=1)# S if the sender is student and A if the sender is alumni
#     Date = models.DateTimeField(auto_now_add=True)
#     chat = models.TextField()
    
#     def __str__(self):
#         return self.stud_username+" "+self.alum_username