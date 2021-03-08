//!APP-HOOK:addMenu
//!MENU-ENTRY:Custom Editor

APP.log("hooked");

if(!APP.customEditorInstalled()){
    APP.log("Adding battle animation editor");

    // list of file extensions this view can edit
    const extensions = ["ANIM"];

    // path to the html to load
    // file path will be concatenated after the "?"
    const prefix = `file://${DATA.projectPath}/tools/BattleAnimationEditor.html?`;

    // add extensions for binary files here
    Object.assign(encoding, {
        "ANIM":null
    });
        
    class BattleAnimationEditorView {
        
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
              className:"BattleAnimationEditorView",
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

    APP.add(new class BattleAnimationEditor{
    
        customEditorInstalled(){ return true; }
    
        pollViewForBuffer( buffer, vf ){
            if( extensions.indexOf(buffer.type) != -1 && vf.priority < 2 ){
                vf.view = BattleAnimationEditorView;
                vf.priority = 2;
            }
        }
    
    }());

}

