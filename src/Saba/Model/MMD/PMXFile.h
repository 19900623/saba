#ifndef SABA_MODEL_PMXFILE_H_
#define SABA_MODEL_PMXFILE_H_

#include "MMDFileString.h"

#include <cstdint>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace saba
{
	template <size_t Size>
	using PMXString = MMDFileString<Size>;

	struct PMXHeader
	{
		PMXString<4>	m_magic;
		float			m_version;

		uint8_t	m_dataSize;

		uint8_t	m_encode;	//0:UTF16 1:UTF8
		uint8_t	m_addUVNum;

		uint8_t	m_vertexIndexSize;
		uint8_t	m_textureIndexSize;
		uint8_t	m_materialIndexSize;
		uint8_t	m_boneIndexSize;
		uint8_t	m_morphIndexSize;
		uint8_t	m_rigidbodyIndexSize;
	};

	struct PMXInfo
	{
		std::string	m_modelName;
		std::string	m_englishModelName;
		std::string	m_comment;
		std::string	m_englishComment;
	};

	/*
	BDEF1
	m_boneIndices[0]

	BDEF2
	m_boneIndices[0-1]
	m_boneWeights[0]

	BDEF4
	m_boneIndices[0-3]
	m_boneWeights[0-3]

	SDEF
	m_boneIndices[0-1]
	m_boneWeights[0]
	m_sdefC
	m_sdefR0
	m_sdefR1

	QDEF
	m_boneIndices[0-3]
	m_boneWeights[0-3]
	*/
	enum class PMXVertexWeight : uint8_t
	{
		BDEF1,
		BDEF2,
		BDEF4,
		SDEF,
		QDEF,
	};

	struct PMXVertex
	{
		glm::vec3	m_position;
		glm::vec3	m_normal;
		glm::vec2	m_uv;

		glm::vec4	m_addUV[4];

		PMXVertexWeight	m_weightType; // 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF 4:QDEF
		int32_t		m_boneIndices[4];
		float		m_boneWeights[4];
		glm::vec3	m_sdefC;
		glm::vec3	m_sdefR0;
		glm::vec3	m_sdefR1;

		float	m_edgeMag;
	};

	struct PMXFace
	{
		uint32_t	m_vertices[3];
	};

	struct PMXTexture
	{
		std::string m_textureName;
	};

	/*
	0x01:���ʕ`��
	0x02:�n�ʉe
	0x04:�Z���t�V���h�E�}�b�v�ւ̕`��
	0x08:�Z���t�V���h�E�̕`��
	0x10:�G�b�W�`��
	0x20:���_�J���[(��2.1�g��)
	0x40:Point�`��(��2.1�g��)
	0x80:Line�`��(��2.1�g��)
	*/
	enum class PMXDrawModeFlags : uint8_t
	{
		BothFace = 0x01,
		GroundShadow = 0x02,
		CastSelfShadow = 0x04,
		RecieveSelfShadow = 0x08,
		DrawEdge = 0x10,
		VertexColor = 0x20,
		DrawPoint = 0x40,
		DrawLine = 0x80,
	};

	/*
	0:����
	1:��Z
	2:���Z
	3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
	*/
	enum class PMXSphereMode : uint8_t
	{
		None,
		Mul,
		Add,
		SubTexture,
	};

	enum class PMXToonMode : uint8_t
	{
		Separate,	//!< 0:��Toon
		Common,		//!< 1:���LToon[0-9] toon01.bmp�`toon10.bmp
	};

	struct PMXMaterial
	{
		std::string	m_name;
		std::string	m_englishName;

		glm::vec4	m_diffuse;
		glm::vec3	m_specular;
		float		m_specularPower;
		glm::vec3	m_ambient;

		PMXDrawModeFlags m_drawMode;

		glm::vec4	m_edgeColor;
		float		m_edgeSize;

		int32_t	m_textureIndex;
		int32_t	m_sphereTextureIndex;
		PMXSphereMode m_sphereMode;

		PMXToonMode	m_toonMode;
		int32_t		m_toonTextureIndex;

		std::string	m_memo;

		int32_t	m_numFaceVertices;
	};

	/*
	0x0001  : �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��

	0x0002  : ��]�\
	0x0004  : �ړ��\
	0x0008  : �\��
	0x0010  : �����

	0x0020  : IK

	0x0080  : ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
	0x0100  : ��]�t�^
	0x0200  : �ړ��t�^

	0x0400  : ���Œ�
	0x0800  : ���[�J����

	0x1000  : ������ό`
	0x2000  : �O���e�ό`
	*/
	enum class PMXBoneFlags : uint16_t
	{
		TargetShowMode = 0x0001,
		AllowRotate = 0x0002,
		AllowTranslate = 0x0004,
		Visible = 0x0008,
		AllowControl = 0x0010,
		IK = 0x0020,
		GiftLocal = 0x0080,
		GiftRotate = 0x0100,
		GiftTranslate = 0x0200,
		FixedAxis = 0x0400,
		LocalAxis = 0x800,
		DeformAfterPhysics = 0x1000,
		DeformOuterParent = 0x2000,
	};

	struct PMXIKLink
	{
		int32_t			m_ikBoneIndex;
		unsigned char	m_enableLimit;

		//m_enableLimit��1�̂Ƃ��̂�
		glm::vec3	m_limitMin;	//���W�A���ŕ\��
		glm::vec3	m_limitMax;	//���W�A���ŕ\��
	};

	struct PMXBone
	{
		std::string	m_name;
		std::string	m_englishName;

		glm::vec3	m_position;
		int32_t		m_parentBoneIndex;
		int32_t		m_deformDepth;

		PMXBoneFlags	m_boneFlag;

		glm::vec3	m_positionOffset;	//�ڑ���:0�̏ꍇ
		int32_t		m_linkBoneIndex;	//�ڑ���:1�̏ꍇ

										//�u��]�t�^�v�܂��́u�ړ��t�^�v���L���̂�
		int32_t	m_giftBoneIndex;
		float	m_giftWeight;

		//�u���Œ�v���L���̂�
		glm::vec3	m_fixedAxis;

		//�u���[�J�����v���L���̂�
		glm::vec3	m_localXAxis;
		glm::vec3	m_localZAxis;

		//�u�O���e�ό`�v���L���̂�
		int32_t	m_keyValue;

		//�uIK�v���L���̂�
		int32_t	m_ikTargetBoneIndex;
		int32_t	m_ikIterationCount;
		float	m_ikLimit;	//���W�A���ŕ\��

		std::vector<PMXIKLink>	m_ikLinks;
	};


	/*
	0:�O���[�v
	1:���_
	2:�{�[��,
	3:UV,
	4:�ǉ�UV1
	5:�ǉ�UV2
	6:�ǉ�UV3
	7:�ǉ�UV4
	8:�ގ�
	9:�t���b�v(��2.1�g��)
	10:�C���p���X(��2.1�g��)
	*/
	enum class PMXMorphType : uint8_t
	{
		Group,
		Position,
		Bone,
		UV,
		AddUV1,
		AddUV2,
		AddUV3,
		AddUV4,
		Material,
		Flip,
		Impluse,
	};

	struct PMXMorph
	{
		std::string	m_name;
		std::string	m_englishName;

		uint8_t			m_controlPanel;	//1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��
		PMXMorphType	m_morphType;

		struct PositionMorph
		{
			int32_t		m_vertexIndex;
			glm::vec3	m_position;
		};

		struct UVMorph
		{
			int32_t		m_vertexIndex;
			glm::vec4	m_uv;
		};

		struct BoneMorph
		{
			int32_t		m_boneIndex;
			glm::vec3	m_position;
			glm::vec4	m_quaternion;
		};

		struct MaterialMorph
		{
			enum class OpType : uint8_t
			{
				Mul,
				Add,
			};

			int32_t		m_materialIndex;
			OpType		m_opType;	//0:��Z 1:���Z
			glm::vec4	m_diffuse;
			glm::vec3	m_specular;
			float		m_specularPower;
			glm::vec3	m_ambient;
			glm::vec4	m_edgeColor;
			float		m_edgeSize;
			glm::vec4	m_textureFactor;
			glm::vec4	m_sphereTextureFactor;
			glm::vec4	m_toonTextureFactor;
		};

		struct GroupMorph
		{
			int32_t	m_morphIndex;
			float	m_weight;
		};

		struct FlipMorph
		{
			int32_t	m_morphIndex;
			float	m_weight;
		};

		struct ImpulseMorph
		{
			int32_t		m_rigidbodyIndex;
			uint8_t		m_localFlag;	//0:OFF 1:ON
			glm::vec3	m_translateVelocity;
			glm::vec3	m_rotateTorque;
		};

		std::vector<PositionMorph>	m_positionMorph;
		std::vector<UVMorph>		m_uvMorph;
		std::vector<BoneMorph>		m_boneMorph;
		std::vector<MaterialMorph>	m_materialMorph;
		std::vector<GroupMorph>		m_groupMorph;
		std::vector<FlipMorph>		m_flipMorph;
		std::vector<ImpulseMorph>	m_impulseMorph;
	};

	struct PMXDispalyFrame
	{

		std::string	m_name;
		std::string	m_englishName;

		enum class TargetType : uint8_t
		{
			BoneIndex,
			MorphIndex,
		};
		struct Target
		{
			TargetType	m_type;
			int32_t		m_index;
		};

		enum class FrameType : uint8_t
		{
			DefaultFrame,	//!< 0:�ʏ�g
			SpecialFrame,	//!< 1:����g
		};

		FrameType			m_flag;
		std::vector<Target>	m_targets;
	};

	struct PMXRigidbody
	{
		std::string	m_name;
		std::string	m_englishName;

		int32_t		m_boneIndex;
		uint8_t		m_group;
		uint16_t	m_collisionGroup;

		/*
		0:��
		1:��
		2:�J�v�Z��
		*/
		enum class Shape : uint8_t
		{
			Sphere,
			Box,
			Capsule,
		};
		Shape		m_shape;
		glm::vec3	m_shapeSize;

		glm::vec3	m_translate;
		glm::vec3	m_rotate;	//���W�A��

		float	m_mass;
		float	m_translateDimmer;
		float	m_rotateDimmer;
		float	m_repulsion;
		float	m_friction;

		/*
		0:�{�[���Ǐ](static)
		1:�������Z(dynamic)
		2:�������Z + Bone�ʒu���킹
		*/
		enum class Operation : uint8_t
		{
			Static,
			Dynamic,
			DynamicAndBoneMerge
		};
		Operation	m_op;
	};

	struct PMXJoint
	{
		std::string	m_name;
		std::string	m_englishName;

		/*
		0:�o�l�t6DOF
		1:6DOF
		2:P2P
		3:ConeTwist
		4:Slider
		5:Hinge
		*/
		enum class JointType : uint8_t
		{
			SpringDOF6,
			DOF6,
			P2P,
			ConeTwist,
			Slider,
			Hinge,
		};
		JointType	m_type;
		int32_t		m_rigidbodyAIndex;
		int32_t		m_rigidbodyBIndex;

		glm::vec3	m_translate;
		glm::vec3	m_rotate;

		glm::vec3	m_translateLowerLimit;
		glm::vec3	m_translateUpperLimit;
		glm::vec3	m_rotateLowerLimit;
		glm::vec3	m_rotateUpperLimit;

		glm::vec3	m_springTranslateFactor;
		glm::vec3	m_springRotateFactor;
	};

	struct PMXSoftbody
	{
		std::string	m_name;
		std::string	m_englishName;

		/*
		0:TriMesh
		1:Rope
		*/
		enum class SoftbodyType : uint8_t
		{
			TriMesh,
			Rope,
		};
		SoftbodyType	m_type;

		int32_t			m_materialIndex;

		uint8_t		m_group;
		uint16_t	m_collisionGroup;

		/*
		0x01:B-Link �쐬
		0x02:�N���X�^�쐬
		0x04: �����N���G
		*/
		enum class SoftbodyMask : uint8_t
		{
			BLink = 0x01,
			Cluster = 0x02,
			HybridLink = 0x04,
		};
		SoftbodyMask	m_flag;

		int32_t	m_BLinkLength;
		int32_t	m_numClusters;

		float	m_totalMass;
		float	m_collisionMargin;

		/*
		1:V_TwoSided
		2:V_OneSided
		3:F_TwoSided
		4:F_OneSided
		*/
		enum class AeroModel : int32_t
		{
			kAeroModelV_TwoSided,
			kAeroModelV_OneSided,
			kAeroModelF_TwoSided,
			kAeroModelF_OneSided,
		};
		int32_t		m_aeroModel;

		//config
		float	m_VCF;
		float	m_DP;
		float	m_DG;
		float	m_LF;
		float	m_PR;
		float	m_VC;
		float	m_DF;
		float	m_MT;
		float	m_CHR;
		float	m_KHR;
		float	m_SHR;
		float	m_AHR;

		//cluster
		float	m_SRHR_CL;
		float	m_SKHR_CL;
		float	m_SSHR_CL;
		float	m_SR_SPLT_CL;
		float	m_SK_SPLT_CL;
		float	m_SS_SPLT_CL;

		//interation
		int32_t	m_V_IT;
		int32_t	m_P_IT;
		int32_t	m_D_IT;
		int32_t	m_C_IT;

		//material
		float	m_LST;
		float	m_AST;
		float	m_VST;

		struct AnchorRigidbody
		{
			int32_t		m_rigidBodyIndex;
			int32_t		m_vertexIndex;
			uint8_t	m_nearMode; //0:FF 1:ON
		};
		std::vector<AnchorRigidbody>	m_anchorRigidbodies;

		std::vector<int32_t>	m_pinVertexIndices;
	};

	struct PMXFile
	{
		PMXHeader	m_header;
		PMXInfo		m_info;

		std::vector<PMXVertex>		m_vertices;
		std::vector<PMXFace>		m_faces;
		std::vector<PMXTexture>		m_textures;
		std::vector<PMXMaterial>	m_materials;
		std::vector<PMXBone>		m_bones;
		std::vector<PMXMorph>		m_morphs;
		std::vector<PMXDispalyFrame>	m_displayFrames;
		std::vector<PMXRigidbody>	m_rigidbodies;
		std::vector<PMXJoint>		m_joints;
		std::vector<PMXSoftbody>	m_softbodies;
	};

	bool ReadPMXFile(PMXFile* pmdFile, const char* filename);
}

#endif // !SABA_MODEL_PMXFILE_H_
