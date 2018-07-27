#include <iostream>
#include <fstream>
#include <string>
#include <aws/s3/S3Client.h>
#include <aws/core/Aws.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/external/tinyxml2/tinyxml2.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/model/DeleteBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>

using namespace Aws::S3;
using namespace Aws::S3::Model;
using namespace std;
using namespace Aws::External::tinyxml2;

int main(int argc, char* argv[]) {
        Aws::SDKOptions options;
        Aws::InitAPI(options);

        Aws::Client::ClientConfiguration cfg;
        cfg.endpointOverride = Aws::String("192.168.10.61:9000");  // S3服务器地址和端口
        cfg.scheme = Aws::Http::Scheme::HTTP;
        Aws::Auth::AWSCredentials cred("minio", "123456789");  // 认证的Key
        Aws::S3::S3Client client(cred, cfg, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never, false);
#if 0
        /**********************CREATE BUCKET*******************************/
        const Aws::String bucket_name = "aaaa";
        Aws::S3::Model::CreateBucketRequest request;
        request.SetBucket(bucket_name);
        auto outcome = client.CreateBucket(request);
        if (outcome.IsSuccess())
        {
                std::cout << "Done!" << std::endl;
        }
        else
        {
                std::cout << "CreateBucket error: "
                        << outcome.GetError().GetExceptionName() << std::endl
                        << outcome.GetError().GetMessage() << std::endl;
        }
        /*****************************************************************/
#endif
#if 1
        /***********************LIST BUCKET*******************************/
        auto response = client.ListBuckets();
        if (response.IsSuccess()) {
                auto buckets = response.GetResult().GetBuckets();
                for (auto iter = buckets.begin(); iter != buckets.end(); ++iter) {
                        cout << iter->GetName() << "\t"  << endl;
                }
        } else {
                cout << "Error while ListBuckets " << response.GetError().GetExceptionName()
                        << " " << response.GetError().GetMessage() << endl;
        }
        /*****************************************************************/

#endif
#if 0

        /*************************DELETE BUCKET***************************/
        std::cout << "Deleting S3 bucket: " << bucket_name << std::endl;


        Aws::S3::Model::DeleteBucketRequest bucket_request;
        bucket_request.SetBucket(bucket_name);

        auto delcome = client.DeleteBucket(bucket_request);

        if (delcome.IsSuccess())
        {
                std::cout << "Done!" << std::endl;
        }
        else
        {
                std::cout << "DeleteBucket error: "
                        << delcome.GetError().GetExceptionName() << " - "
                        << delcome.GetError().GetMessage() << std::endl;
        }
        /*****************************************************************/
#endif
#if 1

        /***************************upload object****************************/
        const Aws::String bucket_name = "wyz";
        const Aws::String key_name = "yyy";
        const Aws::String file_name = "/home/wangyouzhan/minio-deployment.yaml";
        const Aws::String region("");

        std::cout << "Uploading " << file_name << " to S3 bucket " <<
                bucket_name << " at key " << key_name << std::endl;


        Aws::S3::Model::PutObjectRequest object_request;
        object_request.WithBucket(bucket_name).WithKey(key_name);

        // Binary files must also have the std::ios_base::bin flag or'ed in
        auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
                        file_name.c_str(), std::ios_base::in | std::ios_base::binary);

        object_request.SetBody(input_data);

        auto put_object_outcome = client.PutObject(object_request);

        if (put_object_outcome.IsSuccess())
        {
                std::cout << "Done!" << std::endl;
        }
        else
        {
                std::cout << "PutObject error: " <<
                        put_object_outcome.GetError().GetExceptionName() << " " <<
                        put_object_outcome.GetError().GetMessage() << std::endl;
        }
        /*****************************************************************/
#endif
#if 1

        /***************************download object******************************/
        //const Aws::String bucket_name = "wyz";
        //const Aws::String key_name = "yyy";

        std::cout << "Downloading " << key_name << " from S3 bucket: " <<
            bucket_name << std::endl;

        Aws::S3::Model::GetObjectRequest get_object_request;
        get_object_request.WithBucket(bucket_name).WithKey(key_name);

        auto get_object_outcome = client.GetObject(get_object_request);

        if (get_object_outcome.IsSuccess())
        {
            Aws::OFStream local_file;
            local_file.open(key_name.c_str(), std::ios::out | std::ios::binary);
            local_file << get_object_outcome.GetResult().GetBody().rdbuf();
            std::cout << "Done!" << std::endl;
        }
        else
        {
            std::cout << "GetObject error: " <<
                get_object_outcome.GetError().GetExceptionName() << " " <<
                get_object_outcome.GetError().GetMessage() << std::endl;
        }
        /*****************************************************************/
#endif
#if 0

        /*****************************delete object***********************/
        const Aws::String bucket_name = "wyz";
        const Aws::String key_name = "yyy";

        std::cout << "Deleting" << key_name << " from S3 bucket: " <<
            bucket_name << std::endl;


        Aws::S3::Model::DeleteObjectRequest object_request;
        object_request.WithBucket(bucket_name).WithKey(key_name);

        auto delete_object_outcome = client.DeleteObject(object_request);

        if (delete_object_outcome.IsSuccess())
        {
            std::cout << "Done!" << std::endl;
        }
        else
        {
            std::cout << "DeleteObject error: " <<
                delete_object_outcome.GetError().GetExceptionName() << " " <<
                delete_object_outcome.GetError().GetMessage() << std::endl;
        }
        /*****************************************************************/
#endif

        Aws::ShutdownAPI(options);
        return 0;
}

