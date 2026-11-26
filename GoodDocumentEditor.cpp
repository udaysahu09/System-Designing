
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<memory>
using namespace std;

// Abstraction for document elements
class DocumentElement {
public:
    virtual string render() = 0;
    virtual ~DocumentElement() = default;
};

// Concrete implementation for text elements
class TextElement : public DocumentElement {
private:
    string text;

public:
    TextElement(string text) : text(text) {}

    string render() override {
        return text;
    }
};

// Concrete implementation for image elements
class ImageElement : public DocumentElement {
private:
    string imgPath;

public:
    ImageElement(string imgPath) : imgPath(imgPath) {}

    string render() override {
        return "[Image: " + imgPath + "]";
    }
};

// New line element
class NewLineElement : public DocumentElement {
public:
    string render() override {
        return "\n";
    }
};

// Tab space element
class TabSpaceElement : public DocumentElement {
public:
    string render() override {
        return "\t";
    }
};

// Document class
class Document {
private:
    vector<unique_ptr<DocumentElement>> documentElements;

public:
    void addElement(unique_ptr<DocumentElement> element) {
        documentElements.push_back(move(element));
    }

    string render() {
        string result;

        for (auto& element : documentElements) {
            result += element->render();
        }

        return result;
    }
};

// Persistence interface
class Persistence {
public:
    virtual void save(string data) = 0;
    virtual ~Persistence() = default;
};

// File storage implementation
class FileStorage : public Persistence {
public:
    void save(string data) override {
        ofstream outFile("Document.txt");

        if (outFile) {
            outFile << data;
            cout << "Document saved to Document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

// Database storage implementation
class DBStorage : public Persistence {
public:
    void save(string data) override {
        // Save data to database
    }
};

// Document editor
class DocumentEditor {
private:
    Document* document;
    Persistence* storage;

public:
    DocumentEditor(Document* document, Persistence* storage) {
        this->document = document;
        this->storage = storage;
    }

    void addText(string text) {
        document->addElement(
            make_unique<TextElement>(text)
        );
    }

    void addImage(string imagePath) {
        document->addElement(
            make_unique<ImageElement>(imagePath)
        );
    }

    void addNewLine() {
        document->addElement(
            make_unique<NewLineElement>()
        );
    }

    void addSpace() {
        document->addElement(
            make_unique<TabSpaceElement>()
        );
    }

    string renderDocument() {
        return document->render();
    }

    void saveDocument() {
        storage->save(renderDocument());
    }
};

int main() {

    Document document;
    FileStorage fileStorage;

    DocumentEditor editor(&document, &fileStorage);

    editor.addText("Hello, world!");
    editor.addNewLine();

    editor.addText("This is a real-world document editor example.");
    editor.addSpace();
    editor.addNewLine();

    editor.addText("Indented text after a tab space.");
    editor.addNewLine();

    editor.addImage("picture.jpg");

    cout << editor.renderDocument() << endl;

    editor.saveDocument();

    return 0;
}
