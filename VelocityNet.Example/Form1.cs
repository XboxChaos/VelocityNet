using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using VelocityNet.Stfs;

namespace Velocity.Example
{
    public partial class Form1 : Form
    {
        private StfsPackage package;

        public Form1()
        {
            InitializeComponent();
        }

        private void browsePackage_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open STFS Package";
            ofd.Filter = "STFS Packages|*";
            if (ofd.ShowDialog() == DialogResult.Cancel)
                return;

            LoadPackage(ofd.FileName);
        }

        private void packageView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            extractFile.Enabled = replaceFile.Enabled = (e.Node != null);
        }

        private void extractFile_Click(object sender, EventArgs e)
        {
            // Get the selected tree node
            TreeNode selectedNode = packageView.SelectedNode;
            if (selectedNode == null)
                return;

            // Get the file entry from its tag
            StfsFileEntry entry = selectedNode.Tag as StfsFileEntry;
            if (entry == null || entry.IsFolder)
                return;

            // Ask where to save the file
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Title = "Extract File";
            sfd.Filter = "All Files|*.*";
            sfd.FileName = entry.Name;
            if (sfd.ShowDialog() == DialogResult.Cancel)
                return;

            // Extract it
            package.ExtractFile(entry, sfd.FileName);
            MessageBox.Show("File extracted!", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void replaceFile_Click(object sender, EventArgs e)
        {
            // Get the selected tree node
            TreeNode selectedNode = packageView.SelectedNode;
            if (selectedNode == null)
                return;

            // Ask for a file to replace it with
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Replace File";
            ofd.Filter = "All Files|*.*";
            if (ofd.ShowDialog() == DialogResult.Cancel)
                return;

            // Replace it
            string path = GetNodePath(selectedNode);
            package.ReplaceFile(ofd.FileName, path);
            MessageBox.Show("File replaced!", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void saveChanges_Click(object sender, EventArgs e)
        {
            // Ask for a KV
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open KV";
            ofd.Filter = "Binary Files|*.bin|All Files|*.*";
            if (ofd.ShowDialog() == DialogResult.Cancel)
                return;

            // Save!
            package.SaveChanges(ofd.FileName);
            MessageBox.Show("Package saved, rehashed, and resigned!", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void LoadPackage(string path)
        {
            packagePath.Text = path;

            package = new StfsPackage(path);
            ShowListing(package.GetFileListing());
        }

        private void ShowListing(StfsFileListing listing)
        {
            packageView.Nodes.Clear();
            CreateListingNodes(listing, packageView.Nodes);
        }

        private static string GetNodePath(TreeNode node)
        {
            string path = node.Text;
            while (node.Parent != null)
            {
                path += node.Parent.Text + '\\';
                node = node.Parent;
            }
            return path;
        }

        private static void CreateListingNodes(StfsFileListing listing, TreeNodeCollection collection)
        {
            foreach (StfsFileListing folder in listing.Folders)
            {
                TreeNode folderNode = CreateEntryNode(folder.Entry);
                CreateListingNodes(folder, folderNode.Nodes);
            }

            foreach (StfsFileEntry file in listing.Files)
            {
                TreeNode fileNode = CreateEntryNode(file);
                collection.Add(fileNode);
            }
        }

        private static TreeNode CreateEntryNode(StfsFileEntry entry)
        {
            return new TreeNode(entry.Name)
            {
                Tag = entry
            };
        }
    }
}
