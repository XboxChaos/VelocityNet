namespace Velocity.Example
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.packagePath = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.browsePackage = new System.Windows.Forms.Button();
            this.packageView = new System.Windows.Forms.TreeView();
            this.extractFile = new System.Windows.Forms.Button();
            this.replaceFile = new System.Windows.Forms.Button();
            this.saveChanges = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // packagePath
            // 
            this.packagePath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.packagePath.Location = new System.Drawing.Point(101, 12);
            this.packagePath.Name = "packagePath";
            this.packagePath.ReadOnly = true;
            this.packagePath.Size = new System.Drawing.Size(240, 20);
            this.packagePath.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "STFS Package:";
            // 
            // browsePackage
            // 
            this.browsePackage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.browsePackage.Location = new System.Drawing.Point(347, 10);
            this.browsePackage.Name = "browsePackage";
            this.browsePackage.Size = new System.Drawing.Size(75, 23);
            this.browsePackage.TabIndex = 2;
            this.browsePackage.Text = "Browse...";
            this.browsePackage.UseVisualStyleBackColor = true;
            this.browsePackage.Click += new System.EventHandler(this.browsePackage_Click);
            // 
            // packageView
            // 
            this.packageView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.packageView.Location = new System.Drawing.Point(15, 38);
            this.packageView.Name = "packageView";
            this.packageView.Size = new System.Drawing.Size(407, 211);
            this.packageView.TabIndex = 3;
            this.packageView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.packageView_AfterSelect);
            // 
            // extractFile
            // 
            this.extractFile.Enabled = false;
            this.extractFile.Location = new System.Drawing.Point(15, 255);
            this.extractFile.Name = "extractFile";
            this.extractFile.Size = new System.Drawing.Size(75, 23);
            this.extractFile.TabIndex = 4;
            this.extractFile.Text = "Extract...";
            this.extractFile.UseVisualStyleBackColor = true;
            this.extractFile.Click += new System.EventHandler(this.extractFile_Click);
            // 
            // replaceFile
            // 
            this.replaceFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.replaceFile.Enabled = false;
            this.replaceFile.Location = new System.Drawing.Point(96, 255);
            this.replaceFile.Name = "replaceFile";
            this.replaceFile.Size = new System.Drawing.Size(75, 23);
            this.replaceFile.TabIndex = 5;
            this.replaceFile.Text = "Replace...";
            this.replaceFile.UseVisualStyleBackColor = true;
            this.replaceFile.Click += new System.EventHandler(this.replaceFile_Click);
            // 
            // saveChanges
            // 
            this.saveChanges.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.saveChanges.Location = new System.Drawing.Point(312, 255);
            this.saveChanges.Name = "saveChanges";
            this.saveChanges.Size = new System.Drawing.Size(110, 23);
            this.saveChanges.TabIndex = 6;
            this.saveChanges.Text = "Save Changes";
            this.saveChanges.UseVisualStyleBackColor = true;
            this.saveChanges.Click += new System.EventHandler(this.saveChanges_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 290);
            this.Controls.Add(this.saveChanges);
            this.Controls.Add(this.replaceFile);
            this.Controls.Add(this.extractFile);
            this.Controls.Add(this.packageView);
            this.Controls.Add(this.browsePackage);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.packagePath);
            this.Name = "Form1";
            this.Text = "Velocity .NET Example";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox packagePath;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button browsePackage;
        private System.Windows.Forms.TreeView packageView;
        private System.Windows.Forms.Button extractFile;
        private System.Windows.Forms.Button replaceFile;
        private System.Windows.Forms.Button saveChanges;
    }
}

