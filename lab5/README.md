# TECHIN515 Lab 5 - Edge-Cloud Offloading

This project builds upon lab 4. We will implement an edge-cloud offloading strategy, which performs inference or computation locally on ESP32, and offloads it to the cloud (Microsoft Azure) under certain conditions.

The outline of this lab is as follows:
- Train and host a model in Azure
- Deploy the model endpoint as a web app
- Configure magic wand to offload to cloud when uncertain

## Learning Objectives
By completing this lab, students will:
- Be capable of training and hosting models in Azure
- Deploy model as a web app
- Understand cloud-edge offloading

## Project Structure

```
wand-app/
├── wand_model.h5
├── app.py
├── requirements.txt
```

## Hardware Requirements

- Your magic wand from Lab 4

## Software Requirements

### Arduino IDE

- Arduino IDE with ESP32 board support
- Required libraries:
  - Adafruit MPU6050
  - Adafruit Sensor
  - Wire (built-in)

### Microsoft Azure account

### Gesture dataset collected in previous labs

### Python (for data capture)
- Python 3.8 or newer
- Required packages (install via `pip install -r requirements.txt`)

## Getting Started

### 1. Microsoft Azure: Resource Group

1. Navigate to [Microsoft Azure](https://azure.microsoft.com/en-us/) and sign in with your account. If you are new to Azure, you should get $200 free trial credit. 
2. Click on `Resource groups` as shown below to create a resource group.

<img src="assets/resource_group.png" width="800">

3. Name your resource group, e.g., TECHIN515-lab, and choose the closet region, e.g., West US 2. Click `Review+Create`, then `Create` to create the resource group. You can use the default subscription "Azure subscription 1".
   - You would like to have the region close to your location to reduce latency.

### 2. Microsoft Azure: Create a Machine Learning Workspace

1. Navigate back to homepage by clicking on Microsoft Azure on top left, or use this [link](https://portal.azure.com).
2. Click on `Azure Machine Learning` as shown below.

<img src="assets/AzureML.png" width="800">

3. Click on `+Create` to create an AzureML workspace.
   - Use default subscription, i.e., Azure subscription 1
   - Attach the workspace to the resource group you just created
   - Name your workspace, e.g., TECHIN515-lab
   - Use the same region as your resource group
   - Leave storage account, key valut, and application insights as deafult
4. Click on `Review+Create`, then `Create` to create the workspace.

### 3. Microsoft Azure: Create a Compute Instance

There are two types of compute in AzureML: Compute instance and compute cluster. The former is used for development and the latter is used for scalable training jobs.
In the following, we will create a compute instance.

1. Go to your ML workspace and click on `Compute` in the left sidebar.

<img src="assets/compute.png" width="200">

2. Choose the `Compute Instances` tab, and click `+New`.
3. Fill in name, virtual machine size, and region. Note that the region should match that of your workspace.
4. Click `Create`.
It make take a few minutes to create the compute instance.

### 4. Microsoft Azure: Host Data

1. Locate the training data in your laptop. We will host the data in Azure Blob. Note that as the cloud has higher computing performance and more storage, you can get merge your training data with that collected by other students to improve your model performance.
2. Go to AzureML and navigate to Data tab. Click `+Create`.
3. Name you data asset along with a brief description. Choose `Folder (uri_folder)` as type. Click `Next`.
4. Choose From local files option, and upload your training dataset. Click `Next`.
5. Leave datastore type as Azure Blob storage and click `Next`.
6. Upload your training dataset folder. Click `Next` and then `Create`.

### 5. Microsoft Azure: Model Training

1. In the Compute tab, click on JupyterLab.
2. Copy the following code cell to the Jupyter Notebook.
3. Copy the provided code in `train.ipynb` to the Jupyter Notebook in Azure and run the the **first cell**. Once the cell completes running, you should see a `h5` file being created.
4. In the next two cells, you need to configure the following code block accordingly:

```
SUBSCRIPTION = "<subscription_ID>"
RESOURCE_GROUP = "<group_name>"
WS_NAME = "<workspace_name>"
```

To find such information, click the tab on top right showing your subscription and workspace name. Copy Resource Group to `RESOURCE_GROUP`, Subscription ID to `SUBSCIPTION`, and Current workspace to `WS_NAME`.

### 6. Microsoft Azure: Deploy Model via Web App

1. In the left sidebar, search `App Service` and then creat new app. 
   - Choose Python as runtime
   - Region should be picked the same as your ML workspace
   - Use Free plan
2. Create a **local** folder named `wand-app` containing the model (.h5 file).
3. Add the provided `app.py` and `requirements.txt` files to the folder.
4. Push this folder to a GitHub repo, and copy the link to GitHub repo.
5. Go to Azure App Service, and navigate to Deployment Center. Deploy the web app using GitHub actions.

### 7. Cloud-Edge Offloading

1. Complete the provided sketch template. Tune the confidence interval to control when ESP32 should consult to the cloud for gesture inference.
2. Take a picture your serieal monitor for cases when ESP32 performs inference locally, and when ESP32 consults cloud for inference.

## Deliverables

1. GitHub link to your project
2. 