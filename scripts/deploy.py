Import("env")

def after_build(source, target, env):
    # Your custom wireless upload code here
    print("Uploading wirelessly... \n\n\n\n\n\n\n\n\n\n\n")

env.AddPostAction("buildprog", after_build)
