//!APP-HOOK:addMenu
//!MENU-ENTRY:Custom Editor

APP.log("hooked");

if(!APP.customEditorInstalled()){
    APP.log("Adding map editor");

    // list of file extensions this view can edit
    const extensions = ["MAP"];

    // path to the html to load
    // file path will be concatenated after the "?"
    const prefix = `file://${DATA.projectPath}/tools/MapEditor.html?`;

    // add extensions for binary files here
    Object.assign(encoding, {
        "MAP":null
    });
        
    class MapEditorView {
        
        // gets called when the tab is activated
        attach(){
            if( this.DOM.src != prefix + this.buffer.path )
              this.DOM.src = prefix + this.buffer.path;
            this.DOM.contentWindow.readFile = this.readFile;
            this.DOM.contentWindow.saveFile = this.saveFile;
        }

        // file was renamed, update iframe
        onRenameBuffer( buffer ){
            if( buffer == this.buffer ){
                this.DOM.src = prefix + this.buffer.path;
            }
        }
        
        readFile(filePath, mode)
        {
          APP.log("Reading file: " + filePath);
          return window.require("fs").readFileSync(filePath, mode);
        }
        
        saveFile(filePath, data)
        {
          APP.log("Saving file: " + filePath);
          window.require("fs").writeFileSync(filePath, data);
        }
        
        constructor( frame, buffer ){
            this.buffer = buffer;
            this.DOM = DOC.create( frame, "iframe", {
              className:"MapEditorView",
                src: prefix + buffer.path,
                style:{
                    border: "0px none",
                    width: "100%",
                    height: "100%",
                    margin: 0,
                    position: "absolute"
                }
            });
        }
    }

    APP.add(new class MapEditor{
    
        customEditorInstalled(){ return true; }
    
        pollViewForBuffer( buffer, vf ){
            if( extensions.indexOf(buffer.type) != -1 && vf.priority < 2 ){
                vf.view = MapEditorView;
                vf.priority = 2;
            }
        }
    
    }());

}

