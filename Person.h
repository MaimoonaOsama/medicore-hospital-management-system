#ifndef PERSON_H
#define PERSON_H

class Person {
public:
    // Pure virtual methods
    virtual void display() const = 0;
    
protected:
    int id;
    char name[50];
    char password[50];

public:
    Person(int id, const char* name, const char* password);
    virtual ~Person();
  
    virtual bool operator==(const Person& other) const = 0;
    virtual int getId() const { return id; }
    virtual const char* getName() const { return name; }
    virtual const char* getPassword() const { return password; }
    void copyName(const char* source);
    bool compareName(const char* other) const;
};

#endif // PERSON_H