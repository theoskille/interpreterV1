#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

void defineAst(const string& outputDir, const string& baseName, const vector<string>& types);
string getClassName(const string& type);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: generate_ast <output directory>\n";
        exit(65);
    }
    string outputDir = argv[1];
    vector<string> exprAstDef = {
        "Assign: Token name, shared_ptr<Expr> value",
        "Binary: shared_ptr<Expr> left, Token op, shared_ptr<Expr> right",
        "Grouping: shared_ptr<Expr> expression",
        "LiteralExpr: Literal value",
        "Logical: shared_ptr<Expr> left, Token op, shared_ptr<Expr> right",
        "Variable: Token name",
        "Unary: Token op, shared_ptr<Expr> right"
    };
    defineAst(outputDir, "Expr", exprAstDef);

    vector<string> stmtAstDef = {
        "Block: std::vector<shared_ptr<Stmt>> statements",
        "If: shared_ptr<Expr> condition, shared_ptr<Stmt> thenBranch, shared_ptr<Stmt> elseBranch",
        "Expression: shared_ptr<Expr> expression",
        "Var: Token name, shared_ptr<Expr> initializer",
        "Print: shared_ptr<Expr> expression",
        "While: shared_ptr<Expr> condition, shared_ptr<Stmt> body"
    };
    defineAst(outputDir, "Stmt", stmtAstDef);

    return 0;
}

string getClassName(const string& type) {
    size_t colonPos = type.find(": ");
    return type.substr(0, colonPos);
}

void defineAst(const string& outputDir, const string& baseName, const vector<string>& types) {
    string path = outputDir + "/" + baseName + ".h";
    ofstream writer(path);

    // Write the header guards and includes
    writer << "#ifndef " << baseName << "_H\n";
    writer << "#define " << baseName << "_H\n\n";
    
    writer << "#include <memory>\n";
    writer << "#include <vector>\n";
    writer << "#include <string>\n";
    writer << "#include \"Token.h\"\n";
    writer << "#include \"Literal.h\"\n";
    writer << "#include \"Value.h\"\n";
    
    // Include Expr.h if we're generating Stmt.h
    if (baseName == "Stmt") {
        writer << "#include \"Expr.h\"\n";
    }
    writer << "\n";
    
    writer << "using std::shared_ptr;\n\n";
    
    // Forward declare classes
    for (const auto& type : types) {
        writer << "class " << getClassName(type) << ";\n";
    }
    writer << "\n";
    
    // Define visitor class name based on baseName
    string visitorClassName = baseName + "Visitor";
    
    // Define generic visitor interface with templates
    writer << "// Generic visitor interface using templates\n";
    writer << "template<typename R>\n";
    writer << "class " << visitorClassName << " {\n";
    writer << "public:\n";
    writer << "    virtual ~" << visitorClassName << "() = default;\n";
    for (const auto& type : types) {
        string className = getClassName(type);
        string paramName = baseName == "Expr" ? "expr" : "stmt";
        writer << "    virtual R visit" << className << "(" << className << "* " << paramName << ") = 0;\n";
    }
    writer << "};\n\n";
    
    // Define type aliases for common visitor types
    writer << "// Convenience type aliases for common visitor types\n";
    writer << "using " << baseName << "StringVisitor = " << visitorClassName << "<std::string>;\n";
    
    if (baseName == "Expr") {
        writer << "using ValueVisitor = " << visitorClassName << "<Value>;\n\n";
    } else {
        writer << "using VoidVisitor = " << visitorClassName << "<void>;\n\n";
    }
    
    // Define the base class
    writer << "// Base " << (baseName == "Expr" ? "expression" : "statement") << " class\n";
    writer << "class " << baseName << " {\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "() = default;\n";
    writer << "    virtual std::string accept(" << baseName << "StringVisitor& visitor) = 0;\n";
    
    if (baseName == "Expr") {
        writer << "    virtual Value accept(ValueVisitor& visitor) = 0;\n";
    } else {
        writer << "    virtual void accept(VoidVisitor& visitor) = 0;\n";
    }
    writer << "};\n\n";

    // Generate AST classes
    for (const auto& type : types) {
        size_t colonPos = type.find(": ");
        string className = type.substr(0, colonPos);
        string fieldList = type.substr(colonPos + 2);

        writer << "class " << className << " : public " << baseName << " {\n";
        writer << "public:\n";
        
        // Constructor
        writer << "    " << className << "(";
        
        // Parse the field list
        vector<string> fields;
        size_t start = 0;
        size_t commaPos;
        
        while ((commaPos = fieldList.find(", ", start)) != string::npos) {
            fields.push_back(fieldList.substr(start, commaPos - start));
            start = commaPos + 2;
        }
        fields.push_back(fieldList.substr(start));
        
        // Constructor parameters
        for (size_t i = 0; i < fields.size(); i++) {
            string field = fields[i];
            size_t spacePos = field.find(" ");
            string type = field.substr(0, spacePos);
            string name = field.substr(spacePos + 1);
            
            if (i > 0) writer << ", ";
            
            // If type is Literal, pass by value, otherwise by reference
            if (type == "Literal") {
                writer << type << " " << name;
            } else {
                writer << "const " << type << "& " << name;
            }
        }
        writer << ") : ";
        
        // Initialize fields
        for (size_t i = 0; i < fields.size(); i++) {
            string field = fields[i];
            size_t spacePos = field.find(" ");
            string name = field.substr(spacePos + 1);
            
            if (i > 0) writer << ", ";
            writer << name << "(" << name << ")";
        }
        writer << " {}\n\n";
        
        // Implement accept methods for all visitor types
        writer << "    std::string accept(" << baseName << "StringVisitor& visitor) override {\n";
        writer << "        return visitor.visit" << className << "(this);\n";
        writer << "    }\n\n";
        
        if (baseName == "Expr") {
            writer << "    Value accept(ValueVisitor& visitor) override {\n";
            writer << "        return visitor.visit" << className << "(this);\n";
            writer << "    }\n\n";
        } else {
            writer << "    void accept(VoidVisitor& visitor) override {\n";
            writer << "        visitor.visit" << className << "(this);\n";
            writer << "    }\n\n";
        }
        
        // Fields
        writer << "    // Fields\n";
        for (const auto& field : fields) {
            size_t spacePos = field.find(" ");
            string type = field.substr(0, spacePos);
            string name = field.substr(spacePos + 1);
            
            writer << "    " << type << " " << name << ";\n";
        }
        
        writer << "};\n\n";
    }
    
    writer << "#endif // " << baseName << "_H\n";
    writer.close();
    
    cout << "Generated " << path << endl;
}